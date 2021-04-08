
#include "WEB_SERVER/socket_lib/socket_address.h"

#include <iostream>
#include <memory>

#include "WEB_SERVER/socket_lib/socket_exception.h"
#include "WEB_SERVER/utils/debug_tool/debug_common_info.h"

using namespace SONNIE;

socket_info_addr_ipv4::socket_info_addr_ipv4(decltype(INADDR_ANY) _ipv4_address,
                                             int _server_port) {
  addr_ptr_ = std::make_shared<sockaddr_in>();
  memset(addr_ptr_.get(), 0, sizeof(*addr_ptr_));
  addr_ptr_->sin_family = AF_INET;
  // htonl used for 32bits, but htons should be used here?
  addr_ptr_->sin_addr.s_addr = htons(_ipv4_address);
  addr_ptr_->sin_port = htons(_server_port);
};

std::shared_ptr<sockaddr_in> socket_info_addr_ipv4::get_sockaddr() const {
  return addr_ptr_;
}

void socket_info_addr_ipv4::change_ip_port_ipv4(const char *_ipv4_address,
                                                int _server_port) {
  in_addr dest;
  memset(&dest, 0, sizeof(dest));
  int temp = inet_pton(AF_INET, _ipv4_address, &dest);
  if (temp < 0) {
    systemcall_error_info();
    throw socket_exception("faile to transform ipv4 address");
  }
  addr_ptr_->sin_addr = dest;
  addr_ptr_->sin_port = htons(_server_port);
}

socket_info_addr_ipv6::socket_info_addr_ipv6(in6_addr _ipv6_address,
                                             int _server_port) {
  addr_ptr_ = std::make_shared<sockaddr_in6>();
  memset(addr_ptr_.get(), 0, sizeof(*addr_ptr_));
  // the dendianess problem does not exist for array(stackoverflow).?
  addr_ptr_->sin6_addr = _ipv6_address;
  addr_ptr_->sin6_family = AF_INET6;
  addr_ptr_->sin6_port = htons(_server_port);
};

std::shared_ptr<sockaddr_in6> socket_info_addr_ipv6::get_sockaddr() const {
  return addr_ptr_;
}

void socket_info_addr_ipv6::change_ip_port_ipv6(const char *_ipv6_address,
                                                int _server_port) {
  in6_addr dest;
  memset(&dest, 0, sizeof(dest));
  int temp = inet_pton(AF_INET6, _ipv6_address, &dest);
  if (temp < 0) {
    systemcall_error_info();
    throw socket_exception("faile to transform ipv6 address");
  }
  addr_ptr_->sin6_addr = dest;
  addr_ptr_->sin6_port = htons(_server_port);
}