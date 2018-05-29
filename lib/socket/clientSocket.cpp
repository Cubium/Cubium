#include "clientSocket.hpp"
#include "../messages/op_codes.h"
#include "../spa_message.h"
#include <functional> // for std::function
#include <iostream>
#include <netdb.h>      // for h_addr
#include <stdio.h>      // for perror
#include <stdlib.h>     // for exit
#include <strings.h>    // for bzero
#include <sys/socket.h> // for AF_INET, SOL_SOCKET, SO_RCVTIMEO
#include <thread>
#include <unistd.h>

/* Throw a perror and exit */
void clientSocket_error(const char* msg)
{
  std::cout << msg << std::endl;
  perror(msg);
  exit(1);
}

cubiumClientSocket_t clientSocket_openSocket(uint16_t const port)
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
ssize_t clientSocket_requestDialogue(cubiumClientSocket_t* s,                               /* Socket that server is listening through */
                                     void* request,                                         /* Hello sent to server */
                                     size_t const len,                                      /* Size of hello's type */
                                     std::function<void(cubiumClientSocket_t*)> const func, /* Called when ack is received */
                                     const uint8_t targetop)
{
  /* Set timeout such that socket will only wait 50 ms for messages */

  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;

  if (setsockopt(s->sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
  {
    clientSocket_error("setsockopt failed\n");
  }

  uint8_t opcode = 0;
  //std::cout << "Getting called!\n";
  do
  { /* Send a new hello after every timeout until an ack is received */

    s->nBytesRecv = sendto(s->sock, request, len, 0, (const struct sockaddr*)&s->server, s->length);
    if (s->nBytesRecv < 0)
    {
      clientSocket_error("Sendto failed\n");
    }

    recvfrom(s->sock, s->buf, 24, 0, (struct sockaddr*)&s->from, &s->length);
    opcode = ((SpaMessage*)s->buf)->spaHeader.opcode;
    //std::cout << "Got: " << int(opcode) << " from " << ((SpaMessage*)s->buf)->spaHeader.source << "\n";
    //std::cout << "Looking for: " << int(targetop) << "\n";
    if (opcode == op_SPA_SUBSCRIPTION_REQUEST && targetop == op_SPA_SUBSCRIPTION_REPLY)
    {
      func(s);
      return 1;
    }

    /* Continue to send messages until an ack is received */
  } while (opcode != targetop);

  func(s);

  /* Reset the timer to 0 */
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  if (setsockopt(s->sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
  {
    clientSocket_error("setsockopt failed\n");
  }

  return s->nBytesRecv;
}

/* Send a message to the connected server */
ssize_t clientSocket_send(const void* msg, size_t const len, cubiumClientSocket_t* s)
{
  return sendto(s->sock, msg, len, 0, (struct sockaddr*)&s->from, sizeof(struct sockaddr_in));
}

void clientSocket_listen(cubiumClientSocket_t* s, std::function<void(cubiumClientSocket_t*)> const callback, uint8_t exitOp)
{
  //if (exitOp != 0) { std::cout << "I'm waiting!" << std::endl;}
  /* Continually listen for messages and call the handler when one is received */
  for (;;)
  {
    s->nBytesRecv = recvfrom(s->sock, s->buf, 300, 0, (struct sockaddr*)&s->from, &s->length);
    if (s->nBytesRecv < 0)
    {
      clientSocket_error("recvfrom failed");
    }

    /* if exitOp isn't zero, exit if it's the message we're looking for.
     * We use a nested if here because casting messages is expensive. */
    auto opcode = ((SpaMessage*)s->buf)->spaHeader.opcode;
    if (exitOp != 0)
    {
//      std::cout << "Received message with opcode: " << int(opcode) << std::endl;
      if (opcode == exitOp)
      {
        return;
      }
    }

//    std::cout << "Received message with opcode: " << int(opcode) << std::endl;
    callback(s);
  }
}
