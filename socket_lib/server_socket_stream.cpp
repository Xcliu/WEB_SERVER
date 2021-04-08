#include "WEB_SERVER/socket_lib/server_socket_stream.h"

#include <algorithm>
#include <cstring>
#include <iostream>

#include "WEB_SERVER/socket_lib/socket_exception.h"

using namespace SONNIE;

void server_socket_stream::listen_ip_port() {
  int result = listen(server_socket_fd_, backlog_);
  if (result < 0) {
    throw socket_exception("failed to listen socket!");
  }
  return;
}

void server_socket_stream::accept_client_request() {
  socklen_t client_addrLength;
  if (ip_version_ == AF_INET) {
    auto temp = client_socket_info_ipv4_->get_sockaddr();
    client_addrLength = sizeof(*temp);  // the size before casting
    // std::cout<< "fuck lib"<<std::endl;
    server_connected_socket_fd_ = accept(
        server_socket_fd_, (struct sockaddr *)temp.get(), &client_addrLength);
    if (server_connected_socket_fd_ == -1) {
      systemcall_error_info();
      throw socket_exception("failed to accept!");
    }
  } else if (ip_version_ == AF_INET6) {
    auto temp = client_socket_info_ipv6_->get_sockaddr();
    client_addrLength = sizeof(*temp);
    server_connected_socket_fd_ = accept(
        server_socket_fd_, (struct sockaddr *)temp.get(), &client_addrLength);
    if (server_connected_socket_fd_ == -1) {
      systemcall_error_info();
      throw socket_exception("failed to accept!");
    }
  }
  return;
}

const std::string &server_socket_stream::receive_data_from_client(bool) {
  char temp[max_buffer_size_];
  std::fill(temp, temp + max_buffer_size_, '\0');
  int result = recv(server_connected_socket_fd_, temp, max_buffer_size_, 0);
  // the size of recvd_msg_buffer is equal to result,(the size of received msg)
  if (recvd_mesg_buffer_.size() == max_buffer_size_) {
    throw socket_exception("the receive buffer is overflowed");
  } else if (recvd_mesg_buffer_.size() < 0) {
    systemcall_error_info();
    throw socket_exception("fail to receive messages");
  }
  recvd_mesg_buffer_ = temp;
  return recvd_mesg_buffer_;
}

void server_socket_stream::send_short_mesg(const std::string &str) {
  // char buf[520]="HTTP/1.1 200 ok\r\nconnection: close\r\n\r\n";//HTTP响应
  if (str.size() >= max_buffer_size_) {
    throw socket_exception(
        "the size of message is largeer than current buffer");
  }
  int s =
      send(server_connected_socket_fd_, str.c_str(), str.size(), 0);  //发送响应
  if (s < 0) {
    systemcall_error_info();
    throw socket_exception("failed to send short meaasge");
  }
  // 没有区分 request 和 global 级别，这种赋值会降低并发的效率
  // 应该考虑根据 client请求 来构建一个独立的 request级别的结构体
  // 或者不加锁
  send_mesg_buffer_ = str;
  return;
}

void server_socket_stream::send_file_to_client(const int file_fd,
                                               int size) const {
  /*
   *the size of file should be checked.
   */
  if (size < 0) {
    throw socket_exception("the szie of sent file should be positive");
  }
  int temp = ::sendfile(server_connected_socket_fd_, file_fd, nullptr, size);
  if (temp == -1) {
    systemcall_error_info();
    throw socket_exception("failed to send file.");
  }
  return;
}

void server_socket_stream::close_connect_socket() {
  if (close(server_connected_socket_fd_) == -1) {
    systemcall_error_info();
    throw socket_exception("failed to close connected scoket");
  }
}

int server_socket_stream::get_connect_fd() const {
  return server_connected_socket_fd_;
}
