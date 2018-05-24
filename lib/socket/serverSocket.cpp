#include "serverSocket.hpp"
#include "../messages/op_codes.h"
#include "../spa_message.h"
#include <functional>   // for std::function
#include <netinet/in.h> // for INADDR_ANY
#include <stdio.h>      // for perror
#include <stdlib.h>     // for exit
#include <strings.h>    // for bzero
#include <sys/socket.h> // for AF_INET

/* Throw a perror and exit */
void serverSocket_error(const char* msg)
{
  perror(msg);
  exit(1);
}

cubiumServerSocket_t serverSocket_openSocket(uint16_t const port)
{
  cubiumServerSocket_t s;
  s.sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (s.sock < 0)
  {
    serverSocket_error("Failed to open socket");
  }
  s.length = sizeof(s.server);
  bzero(&s.server, s.length);
  s.server.sin_family = AF_INET;
  s.server.sin_addr.s_addr = INADDR_ANY;
  s.server.sin_port = htons(port);

  if (bind(s.sock, (struct sockaddr*)&s.server, s.length) < 0)
  {
    serverSocket_error("Failed to bind socket");
  }

  s.fromlen = sizeof(struct sockaddr_in);

  return s;
}

/* Listen through the given socket */
void serverSocket_listen(cubiumServerSocket_t* s, std::function<int(cubiumServerSocket_t*)> const callback)
{
  /* Continually listen for messages and call the handler when one is received */
  for (;;)
  {
    s->nBytesRecv = recvfrom(s->sock, s->buf, 300, 0, (struct sockaddr*)&s->from, &s->fromlen);
    if (s->nBytesRecv < 0)
    {
      serverSocket_error("recvfrom failed");
    }

    if (callback(s) != 0)
    {
      return;
    }
  }
}

/* Send a message through the socket */
ssize_t serverSocket_send(const void* msg, size_t const len, cubiumServerSocket_t* s)
{
  return sendto(s->sock, msg, len, 0, (struct sockaddr*)&s->from, s->fromlen);
}
