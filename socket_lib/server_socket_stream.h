#pragma once

#include <string>

#include "WEB_SERVER/socket_lib/server_socket.h"
#include "socket_common_define.h"

namespace SONNIE {

class server_socket_stream : public server_socket {
 private:
  // how many clients cloud be blocked in one server socket
  const int backlog_;
  int server_connected_socket_fd_ = -1;

 public:
  server_socket_stream(decltype(AF_INET) ip_version = AF_INET,
                       int client_num = 1)
      : server_socket(ip_version, SOCK_STREAM), backlog_(client_num){};
  virtual ~server_socket_stream() = default;
  server_socket_stream(const server_socket_stream&) = delete;
  server_socket_stream& operator=(const server_socket_stream&) = delete;

  void listen_ip_port();
  void accept_client_request();
  const std::string& receive_data_from_client(bool) override;
  void send_short_mesg(const std::string& str) override;
  void send_file_to_client(int file_fd, int size) const;
  void close_connect_socket();
  int get_connect_fd() const;
  std::string get_client_info_ipv4() const;
  std::string get_client_info_ipv6() const;
};

}  // namespace SONNIE
