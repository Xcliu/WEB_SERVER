#include "server_socket_datagram.h"
#include "socket_related_head_file.h"
#include "socket_exception.h"
#include <iostream>
#include <string>
using namespace SONNIE; 

// int main(){
//     std::cout << "********WEB client(ipv6)**********" <<std::endl;
//     try{
        
//         server_socket_datagram *server_socket_obj=new server_socket_datagram(AF_INET6);
//         server_socket_obj->create_socket();
//         std::string str_send="fucking, udp server!";
        
//         //server_socket_obj->set_client_addr_ipv6(ipv4_addr,9527);
        
//         server_socket_obj->send_short_mesg(str_send);
//     }catch (socket_exception &exp_s){
//             std::cout << exp_s.what()<<std::endl;
//     };
//     return 0;
// }


int main(){
    std::cout << "******** WEB client(ipv6) **********" <<std::endl;
    try{
        
        server_socket_datagram *server_socket_obj=new server_socket_datagram(AF_INET6);
        server_socket_obj->create_socket();
        std::string str_send="fucking, udp server!";
        server_socket_obj->set_client_addr_ipv6("::2",8888);
        server_socket_obj->send_short_mesg(str_send);
    }catch (socket_exception &exp_s){
            std::cout << exp_s.what()<<std::endl;
    };
    return 0;
}