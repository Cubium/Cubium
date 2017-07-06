#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "socket_address.hpp"

class Socket
{
public:
  Socket() : fd(-1)
  {
    if (init() == false)
    {
      handleFailure();
    }
  }

  virtual ~Socket()
  {
    if (fd >= 0)
    {
      close(fd);
    }
  }

  //! Send message to port

  //! \param address - ip address of destination
  //! \return true if successful, false otherwise
  virtual bool send(std::string address, uint16_t port, uint8_t *buff, uint32_t buffLen)
  {
    if (fd < 1)
    {
      handleFailure();
    }

    SocketAddress addr(address, port);
    struct sockaddr_in saddr = addr.to_sockaddr_in();
    int8_t result = sendto(
        fd,
        buff,
        buffLen,
        0,
        (struct sockaddr *)&saddr,
        sizeof(saddr));

    if (result == -1)
    {
      handleFailure();
      return false;
    }
    return true;
  }

protected:
  virtual bool init()
  {
    if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
      return false;
    }
    return true;
  }

  virtual void handleFailure()
  {
    // TODO not sure how to die just yet :)
    std::cout << "Socket Failure" << std::endl;
    exit(1);
  }

  int32_t fd;
};

#endif
