/*
    Simple udp server
*/

#include "../../lib/platform_abstraction/socket/server_socket.hpp"

#define BUFLEN 512 //Max length of buffer
#define PORT 8888  //The port on which to listen for incoming data

void handleConnection(uint8_t *buf, uint32_t buflen)
{
  printf("Data: %s\n", buf);
  delete[] buf;
}

int main(void)
{
  ServerSocket s;

  s.bindSocket(PORT);
  s.listen(handleConnection);
  return 0;
}
