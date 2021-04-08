#include "WEB_SERVER/socket_lib/server_socket.h"

#include <memory>

#include "WEB_SERVER/socket_lib/socket_exception.h"
#include "WEB_SERVER/utils/debug_tool/debug_common_info.h"
using namespace SONNIE;

void server_socket::init_socket() {
  if (socket_type_ == SOCK_DGRAM) {
    /*not necessary*/
    server_socket_fd_ = socket(ip_version_, socket_type_, IPPROTO_UDP);
  } else if (socket_type_ == SOCK_STREAM) {
    server_socket_fd_ = socket(ip_version_, socket_type_, 0);
  }
  // std::cout<< server_socket_fd <<std::endl;
  if (server_socket_fd_ < 0) {
    systemcall_error_info();
    throw socket_exception("failed to create a socket");
  }
  return;
}

void server_socket::bind_socket_to_ipv4_port(const char *ipv4_address,
                                             int server_port) {
  if (ip_version_ != AF_INET) {
    throw socket_exception(
        "non_ipv4_socket can not be binded to ipv4 address!");
  }
  if (strcmp(ipv4_address, "") != 0 && server_port != 0) {
    server_socket_info_ipv4_->change_ip_port_ipv4(ipv4_address, server_port);
  }

  const auto temp = server_socket_info_ipv4_->get_sockaddr();
  socklen_t socket_size = sizeof(*temp);
  int result =
      bind(server_socket_fd_, (struct sockaddr *)temp.get(), socket_size);

  if (result < 0) {
    systemcall_error_info();
    throw socket_exception("failed to bind socket!");
  }
  return;
}

void server_socket::bind_socket_to_ipv6_port(const char *ipv6_address,
                                             int server_port) {
  if (ip_version_ != AF_INET6) {
    throw socket_exception(
        "non_ipv6_socket can not be binded to ipv6 address!");
  }
  if (strcmp(ipv6_address, "") != 0 && server_port != 0) {
    // std::cout << "check failed  "<<std::endl;
    server_socket_info_ipv6_->change_ip_port_ipv6(ipv6_address, server_port);
  }
  const auto temp = server_socket_info_ipv6_->get_sockaddr();
  socklen_t socket_size = sizeof(*temp);
  int result =
      bind(server_socket_fd_, (struct sockaddr *)temp.get(), socket_size);
  if (result < 0) {
    systemcall_error_info();
    throw socket_exception("failed to bind socket!");
  }
  return;
}

void server_socket::close_server_socket() const {
  if (close(server_socket_fd_) == -1) {
    throw socket_exception("failed to close connected scoket");
  }
}

server_socket::~server_socket() {
  if (server_socket_fd_ != -1) {
    close_server_socket();
  }
}

std::string server_socket::get_client_info(decltype(AF_INET) ip_version) const {
  if (ip_version == AF_INET) {
    if (client_socket_info_ipv4_ == nullptr) {
      return "";
    }
    auto temp = client_socket_info_ipv4_->get_sockaddr();

    // checking codes which should be removed before using
    // temp->sin_addr.s_addr=INADDR_ANY;
    // temp->sin_port=htons(4242);

    char dst[max_buffer_size_];
    if (inet_ntop(AF_INET, &(temp->sin_addr), dst, max_buffer_size_) ==
        nullptr) {
      systemcall_error_info();
      throw socket_exception("fail to tranfrom ip to string");
    }
    return std::string(dst) + ";" + std::to_string(ntohs(temp->sin_port));
  } else if (ip_version == AF_INET6) {
    if (client_socket_info_ipv6_ == nullptr) {
      return "";
    }
    auto temp = client_socket_info_ipv6_->get_sockaddr();

    // checking codes which should be removed before using
    // (temp->sin6_addr)=in6addr_any;
    // temp->sin6_port=htons(4243);

    char dst[max_buffer_size_];
    if (inet_ntop(AF_INET6, &(temp->sin6_addr), dst, max_buffer_size_) ==
        nullptr) {
      systemcall_error_info();
      throw socket_exception("fail to tranfrom ip to string");
    }
    return std::string(dst) + ";" + std::to_string(ntohs(temp->sin6_port));
  }
  return "";
}
