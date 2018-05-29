#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include <functional>   // for std::function
#include <netinet/in.h> // for sockaddr_in
#include <stdint.h>     // for uint32_t, uint16_t
#include <sys/socket.h> // for socklen_t
#include <sys/types.h>  // for ssize_t

/* Everything we need for socket communication bundled into a struct */
typedef struct
{
  int32_t sock;
  uint32_t nBytesRecv;
  uint32_t length;
  socklen_t fromlen;
  struct sockaddr_in server;
  struct sockaddr_in from;
  char buf[1024];
} cubiumServerSocket_t;

/* Send a message through the socket */
ssize_t serverSocket_send(const void* msg,        /* Message buffer */
                          size_t const len,       /* Length of message */
                          cubiumServerSocket_t* s /* Socket */
                          );

/* Start listening through a given socket*/
void serverSocket_listen(cubiumServerSocket_t* port,                           /* Socket to listen through */
                         std::function<int(cubiumServerSocket_t*)> const func /* Function called when a message is received */
                         );

/* Open a socket on the given port. Returns the socket info */
cubiumServerSocket_t serverSocket_openSocket(uint16_t const port);

#endif
