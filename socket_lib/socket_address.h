#pragma once

#include <memory>

#include "WEB_SERVER/socket_lib/socket_common_define.h"

namespace SONNIE {

class socket_info_addr_ipv4 {
 private:
  // useed for saving ipv4 address (struct type)
  std::shared_ptr<sockaddr_in> addr_ptr_;

 public:
  // struct type (in_addr) containing the ipv4 address
  explicit socket_info_addr_ipv4(
      decltype(INADDR_ANY) _ipv4_address = INADDR_ANY, int _server_port = 8888);
  socket_info_addr_ipv4(const socket_info_addr_ipv4&) = delete;
  socket_info_addr_ipv4& operator=(const socket_info_addr_ipv4&) = delete;
  virtual ~socket_info_addr_ipv4() = default;

  std::shared_ptr<sockaddr_in> get_sockaddr() const;
  void change_ip_port_ipv4(const char* _ipv4_address, int _server_port);
};

class socket_info_addr_ipv6 {
 private:
  // useed for saving ipv6 address (struct type)
  std::shared_ptr<sockaddr_in6> addr_ptr_;

 public:
  socket_info_addr_ipv6(in6_addr _ipv6_address = in6addr_any,
                        int _server_port = 8888);
  virtual ~socket_info_addr_ipv6() = default;
  socket_info_addr_ipv6(const socket_info_addr_ipv4&) = delete;
  socket_info_addr_ipv6& operator=(const socket_info_addr_ipv6&) = delete;

  std::shared_ptr<sockaddr_in6> get_sockaddr() const;
  void change_ip_port_ipv6(const char* _ipv6_address, int _server_port);
};

}  // namespace SONNIE
