#ifndef SERVER_SOCKET_DATAGRAM_H_
#define SERVER_SOCKET_DATAGRAM_H_

#include <string>

#include "WEB_SERVER/socket_lib/server_socket.h"

namespace SONNIE {

class server_socket_datagram : public server_socket {
 public:
  server_socket_datagram(int _ip_version = AF_INET);
  virtual ~server_socket_datagram() = default;
  const std::string &receive_data_from_client(bool save_client_addr) override;
  void send_short_mesg(const std::string &str) override;
  void set_client_addr_ipv4(const char *_ipv4_address, int _server_port);
  void set_client_addr_ipv6(const char *_ipv6_address, int _server_port);
};

}  // namespace SONNIE

#endif