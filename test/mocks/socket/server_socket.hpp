#include <platform_abstraction/socket/server_socket.hpp>
class MockServerSocket : public ServerSocket
{
public:
  MOCK_METHOD4(send, bool(std::string, uint16_t, uint8_t *, uint32_t));
  MOCK_METHOD1(listen, void(ServerSocket::MessageCallback));
};
