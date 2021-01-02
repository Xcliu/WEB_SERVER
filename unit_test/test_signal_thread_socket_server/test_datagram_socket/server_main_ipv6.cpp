#include "server_socket_datagram.h"
#include "socket_related_head_file.h"
#include "socket_exception.h"
#include <iostream>
#include <string>

using namespace SONNIE; 


int main(){
    std::cout << "************ WEB server(ipv6) ************" <<std::endl;
    try{
        server_socket_datagram *server_socket_obj=new server_socket_datagram(AF_INET6);
        server_socket_obj->create_socket();

        // in6_addr temp=in6addr_any;
        // char dst[sizeof(temp)];
        // inet_ntop(AF_INET6,&(temp),dst,sizeof(temp));
        server_socket_obj->bind_socket_to_ipv6_port();
        std::string received_mesg=server_socket_obj->receive_data_from_client(0);
        std::cout<< "server received messages: "<<received_mesg <<std::endl;
        std::cout<< "client address info: " << server_socket_obj->get_client_info(AF_INET6)<<std::endl;
    }catch (socket_exception &exp_s){
            std::cout << exp_s.what()<<std::endl;
    };
    return 0;
}
