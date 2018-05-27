#ifndef CLIENT_SOCKET_HPP
#define CLIENT_SOCKET_HPP

#include <functional>   // for std::function
#include <netinet/in.h> // for sockaddr_in
#include <stdint.h>     // for uint32_t
#include <sys/types.h>  // for ssize_t

/* Everything we need for socket communication bundled up into a struct */
typedef struct
{
  int32_t sock;
  uint32_t nBytesRecv;
  uint32_t length;
  struct sockaddr_in server;
  struct sockaddr_in from;
  struct hostent* hp;
  char buf[256];
} cubiumClientSocket_t;

/* Open a socket on the given port. Returns socket info. */
cubiumClientSocket_t clientSocket_openSocket(uint16_t const port);

/* Connect to a UDP server */
ssize_t clientSocket_requestDialogue(cubiumClientSocket_t*,                                 /* Socket that server is listening through */
                                     void* hello,                                           /* Hello sent to server */
                                     size_t const len,                                      /* Length of hello */
                                     std::function<void(cubiumClientSocket_t*)> const func, /* Called when ack is received */
                                     const uint8_t targetop);

/* Send a message through the socket */
ssize_t clientSocket_send(const void* msg,        /* The message buffer */
                          size_t const len,       /* Message length */
                          cubiumClientSocket_t* s /* Socket */
                          );

void clientSocket_listen(cubiumClientSocket_t* s, std::function<void(cubiumClientSocket_t*)> const func, uint8_t exitOp);

#endif
