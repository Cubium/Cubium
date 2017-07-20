#include "clientSocket.hpp"
#include "../spa_message.h"
#include <functional>   // for std::function
#include <netdb.h>      // for h_addr
#include <stdio.h>      // for perror
#include <stdlib.h>     // for exit
#include <strings.h>    // for bzero
#include <sys/socket.h> // for AF_INET, SOL_SOCKET, SO_RCVTIMEO
#include <iostream>
#include <thread>

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
void clientSocket_serverConnect(cubiumClientSocket_t* s,                        /* Socket that server is listening through */
                                void* hello,                                    /* Hello sent to server */
                                size_t len,                                     /* Size of hello's type */
                                std::function<void(cubiumClientSocket_t*)> func /* Called when ack is received */
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

  do
  { /* Send a new hello after every timeout until an ack is received */

    s->nBytesRecv = sendto(s->sock, hello, len, 0, (const struct sockaddr*)&s->server, s->length);
    if (s->nBytesRecv < 0)
    {
      clientSocket_error("Sendto failed\n");
    }

  } while (recvfrom(s->sock, s->buf, 24, 0, (struct sockaddr*)&s->from, &s->length) < 0);

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

void clientSocket_listen(cubiumClientSocket_t* s, std::function<void(cubiumClientSocket_t*)> func)
{
  /* Continually listen for messages and call the handler when one is received */
  while (true)
  {
    std::cout << "Listening!" << std::endl;
    s->nBytesRecv = recvfrom(s->sock, s->buf, 24, 0, (struct sockaddr*)&s->from, &s->length);
    if (s->nBytesRecv < 0)
    {
      clientSocket_error("recvfrom failed");
    }
    func(s);
  }
}
