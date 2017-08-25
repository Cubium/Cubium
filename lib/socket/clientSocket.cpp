#include "clientSocket.hpp"
#include "../spa_message.h"
#include "../messages/op_codes.h"
#include "../messages/spa/spa_courier.h"
#include <functional> // for std::function
#include <iostream>
#include <netdb.h>      // for h_addr
#include <stdio.h>      // for perror
#include <stdlib.h>     // for exit
#include <strings.h>    // for bzero
#include <sys/socket.h> // for AF_INET, SOL_SOCKET, SO_RCVTIMEO
#include <thread>

//#define CLIENTSOCKET_VERBOSE

/* Throw a perror and exit */
void clientSocket_error(const char* msg)
{
  perror(msg);
  exit(1);
}

cubiumClientSocket_t clientSocket_openSocket(uint16_t port)
{
  cubiumClientSocket_t s;

  s.sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (s.sock < 0)
  {
    clientSocket_error("socket");
  }
  s.server.sin_family = AF_INET;
  s.hp = gethostbyname("localhost");
  if (0 == s.hp)
  {
    clientSocket_error("Unknown host");
  }
  bcopy((char*)s.hp->h_addr,
        (char*)&s.server.sin_addr,
        s.hp->h_length);
  s.server.sin_port = htons(port);
  s.length = sizeof(struct sockaddr_in);
  bzero(s.buf, 256);

  return s;
}

/* Connect to a UDP server */
void clientSocket_requestDialogue(cubiumClientSocket_t* s,                        /* Socket that server is listening through */
                                void* request,                                    /* Hello sent to server */
                                size_t len,                                     /* Size of hello's type */
                                std::function<void(cubiumClientSocket_t*)> func, /* Called when ack is received */
                                const uint8_t targetop 
                                )
{
  /* Set timeout such that socket will only wait 5 ms for messages */

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 5000;

  if (setsockopt(s->sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
  {
    clientSocket_error("setsockopt failed\n");
  }

  uint8_t opcode = 0;
  do
  { /* Send a new hello after every timeout until an ack is received */

    s->nBytesRecv = sendto(s->sock, request, len, 0, (const struct sockaddr*)&s->server, s->length);
    if (s->nBytesRecv < 0)
    {
      clientSocket_error("Sendto failed\n");
    }

    recvfrom(s->sock, s->buf, 24, 0, (struct sockaddr*)&s->from, &s->length);
    opcode = ((SpaMessage*)s->buf)->spaHeader.opcode;

  } while (opcode != targetop);

  /* Ack received; call handler function */
  func(s);

  /* Reset the timer to 0 */
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  if (setsockopt(s->sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
  {
    clientSocket_error("setsockopt failed\n");
  }
}

/* Send a message to the connected server */
ssize_t clientSocket_send(const void* msg, size_t len, cubiumClientSocket_t* s)
{
  return sendto(s->sock, msg, len, 0, (struct sockaddr*)&s->from, sizeof(struct sockaddr_in));
}

void clientSocket_handleCourier(cubiumClientSocket_t* s, std::function<void(cubiumClientSocket_t*)> func, SpaCourier * courier)
{
  std::cout << "Client handling courier" << std::endl;
  do
  {
    s->nBytesRecv = recvfrom(s->sock, s->buf, courier->followerSize, 0, (struct sockaddr*)&s->from, &s->length);
    if (s->nBytesRecv < 0)
    {
      clientSocket_error("recvfrom failed");
    }
  } while (s->nBytesRecv != courier->followerSize);

  std::cout << "Got buffer: " << s->buf << std::endl;

}

void clientSocket_listen(cubiumClientSocket_t* s, std::function<void(cubiumClientSocket_t*)> func)
{
  /* Continually listen for messages and call the handler when one is received */
  for(;;)
  {
#ifdef CLIENTSOCKET_VERBOSE
    std::cout << "Listening!" << std::endl;
#endif
    s->nBytesRecv = recvfrom(s->sock, s->buf, 40, 0, (struct sockaddr*)&s->from, &s->length);
    if (s->nBytesRecv < 0)
    {
      clientSocket_error("recvfrom failed");
    }

    /* Special check for handling spa couriers */
    if (((SpaMessage*)s->buf)->spaHeader.opcode == op_SPA_COURIER)
    {
      clientSocket_handleCourier(s, func, (SpaCourier*)s->buf);
      return;
    }

    func(s);
  }
}
