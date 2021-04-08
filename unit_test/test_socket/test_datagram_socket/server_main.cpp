#include <iostream>
#include <memory>
#include <string>

#include "WEB_SERVER/socket_lib/server_socket_datagram.h"
#include "WEB_SERVER/socket_lib/socket_exception.h"

using namespace SONNIE;

int main() {
  std::cout << "*********WEB server**********" << std::endl;

  try {
    std::shared_ptr<server_socket_datagram> server_socket_obj{
        new server_socket_datagram(AF_INET)};
    server_socket_obj->init_socket();
    server_socket_obj->bind_socket_to_ipv4_port("127.0.0.1", 8888);

    while (true) {
      std::string received_mesg =
          server_socket_obj->receive_data_from_client(true);
      std::cout << "server received messages: \n" << received_mesg << std::endl;
      std::cout << "client address info: "
                << server_socket_obj->get_client_info(AF_INET) << std::endl;
    }
  } catch (socket_exception &exp_s) {
    std::cout << exp_s.what() << std::endl;
  };

  return 0;
}
