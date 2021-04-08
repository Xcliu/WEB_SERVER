#include <iostream>
#include <string>

#include "WEB_SERVER/socket_lib/server_socket_datagram.h"
#include "WEB_SERVER/socket_lib/socket_exception.h"
using namespace SONNIE;

int main() {
  std::cout << "********WEB client**********" << std::endl;
  try {
    server_socket_datagram *server_socket_obj =
        new server_socket_datagram(AF_INET);
    server_socket_obj->init_socket();
    std::string str_send = "message from client, your fucking udp server!";
    // 注意此时从当前 “client” 的角度看，它需要连接的 server 就是它的 client
    server_socket_obj->set_client_addr_ipv4("127.0.0.1", 8888);
    server_socket_obj->send_short_mesg(str_send);
  } catch (socket_exception &exp_s) {
    std::cout << exp_s.what() << std::endl;
  };
  return 0;
}