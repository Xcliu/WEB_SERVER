#include "server_socket_datagram.h"
#include "socket_related_head_file.h"
#include "socket_exception.h"
#include <iostream>
#include <string>

using namespace SONNIE; 

// int main(){
//     std::cout << "************WEB server(ipv6)************" <<std::endl;
//     try{
//         server_socket_datagram *server_socket_obj=new server_socket_datagram(AF_INET6);
//         server_socket_obj->create_socket();

//         in6_addr *dest=new in6_addr{0};
//         int s=inet_pton(AF_INET6,"::1",dest);
//         // inet_pton(AF_INET6,"0:0:0:0:0:0:0x2:0x1",dest);//inet_addr("127.0.0.1");
//         // std::cout << _TYPEINFO(IN6ADDR_ANY_INIT))<<std::endl;
//         server_socket_obj->bind_socket_to_ipv6_port(in6addr_any,9527);
//         std::string received_mesg=server_socket_obj->receive_data_from_client(0);
//         std::cout<< "server received messages: "<<received_mesg <<std::endl;   
//     }catch (socket_exception &exp_s){
//             std::cout << exp_s.what()<<std::endl;
//     };
//     return 0;
// }

int main(){
    std::cout << "************ WEB server(ipv6) ************" <<std::endl;
    try{
        server_socket_datagram *server_socket_obj=new server_socket_datagram(AF_INET6);
        server_socket_obj->create_socket();

        in6_addr temp=in6addr_any;
        char dst[sizeof(temp)];
        inet_ntop(AF_INET6,&(temp),dst,sizeof(temp));
        
        server_socket_obj->bind_socket_to_ipv6_port(dst,8888);
        std::string received_mesg=server_socket_obj->receive_data_from_client(0);
        std::cout<< "server received messages: "<<received_mesg <<std::endl;
    }catch (socket_exception &exp_s){
            std::cout << exp_s.what()<<std::endl;
    };
    return 0;
}
