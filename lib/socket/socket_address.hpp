#ifndef SOCKET_ADDRESS_HPP
#define SOCKET_ADDRESS_HPP

#include <arpa/inet.h>

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

class SocketAddress
{
public:
  SocketAddress(std::string addr = "", int32_t port = 0, uint32_t s_addr = 0) : address(addr), port(port) {}

  void handleFailure()
  {
    std::cout << "Socket Address Failure" << std::endl;
    exit(1);
  }

  struct sockaddr_in to_sockaddr_in()
  {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (s_addr > -1)
    {
      addr.sin_addr.s_addr = htonl(s_addr);
    }
    if (address.length() > 0)
    {
      if (inet_aton(address.c_str(), &addr.sin_addr) == 0)
      {
        handleFailure();
      }
    }

    return addr;
  }

protected:
  std::string address;
  int32_t port;
  uint32_t s_addr;
};

#endif
