#include "server_socket_datagram.h"
#include "socket_related_head_file.h"
#include "socket_exception.h"
#include <iostream>
#include <string>

using namespace SONNIE; 

int main(){
    std::cout << "*********WEB server**********" <<std::endl;
    try{
        server_socket_datagram *server_socket_obj=new server_socket_datagram(AF_INET);
        server_socket_obj->create_socket();
        server_socket_obj->bind_socket_to_ipv4_port("127.0.0.1",8888);
        std::string received_mesg=server_socket_obj->receive_data_from_client(0);
        std::cout<< "server received messages: \n"<<received_mesg <<std::endl;
    }catch (socket_exception &exp_s){
            std::cout << exp_s.what()<<std::endl;
    };
    return 0;
}


