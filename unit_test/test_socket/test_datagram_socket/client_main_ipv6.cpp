#include "WEB_SERVER/socket_lib/server_socket_datagram.h"
#include "WEB_SERVER/socket_lib/socket_exception.h"
#include <iostream>
#include <string>
using namespace SONNIE; 

int main(){
    std::cout << "******** WEB client(ipv6) **********" <<std::endl;
    try{
        server_socket_datagram *server_socket_obj=new server_socket_datagram(AF_INET6);
        server_socket_obj->init_socket();
        std::string str_send="fucking, udp server!";
        // BUG(lxc): there is an error about seting the ipv6 address 
        server_socket_obj->set_client_addr_ipv6("fec0::da24:bdff:fe76:cea2",8888);
        server_socket_obj->send_short_mesg(str_send);
    }catch (socket_exception &exp_s){
            std::cout << exp_s.what()<<std::endl;
    };
    
    return 0;
}