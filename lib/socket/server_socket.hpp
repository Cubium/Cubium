#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP
#include <stdio.h>  //printf
#include <string.h> //memset
#include <thread>
#include <functional>
#include "socket.hpp"

#define BUFLEN 512 //Max length of buffer

class ServerSocket : public Socket
{
public:
  ServerSocket() : Socket() {}

  typedef void (*MessageCallback)(uint8_t *, uint32_t);

  virtual bool bindSocket(uint32_t port)
  {
    //TODO check fd for errors

    SocketAddress addr("", port);
    struct sockaddr_in si_me = addr.to_sockaddr_in();
    if (bind(fd, (struct sockaddr *)&si_me, sizeof(si_me)) == -1)
    {
      handleFailure();
      return false;
    }
    return true;
  }

  virtual void listen(std::function<void(uint8_t *, uint32_t)> connectionHandler)
  {
    //TODO check fd for errors
    uint8_t buf[BUFLEN];

    SocketAddress addr;
    struct sockaddr_in si_other = addr.to_sockaddr_in();

    for (;;)
    {
      memset(buf, 0, BUFLEN);
      fflush(stdout);

      uint32_t addr_len = sizeof(si_other);

      //try to receive some data, this is a blocking call
      size_t result = recvfrom(
          fd,
          buf,
          BUFLEN,
          0,
          (struct sockaddr *)&si_other,
          &addr_len);

      if (result == -1)
      {
        handleFailure();
      }

      uint8_t *buffCopy = new uint8_t[BUFLEN];
      memcpy(buffCopy, buf, BUFLEN);

      new std::thread(connectionHandler, buffCopy, BUFLEN);
    }
  }
};
#endif
