#include "server_socket_datagram.h"
#include "socket_related_head_file.h"
#include "socket_exception.h"
#include <iostream>
#include <string>

using namespace SONNIE; 

int main(){
    std::cout << "hello world WEB server!" <<std::endl;
    try{
        server_socket_datagram server_socket_obj(AF_INET);
        std::cout <<"fuck main again 1" <<std::endl;
        //server_socket_datagram server_socket_obj(AF_INET);
        // server_socket_obj.create_socket();
        // server_socket_obj.bind_socket_to_ipv4_port(INADDR_ANY,8000);
        // while(1){

        //     //server_socket_obj.set_client_addr_ipv4(INADDR_ANY,8888);
        //     std::string str_send="hello, server!";
        //     server_socket_obj.send_short_mesg(str_send);

        //     std::string received_mesg=server_socket_obj.receive_data_from_client(0);
        //     std::cout<< "client received messages: \n"<<received_mesg <<std::endl;
        // }
    }catch (socket_exception &exp_s){
        std::cout <<"fuck main again 2" <<std::endl;
        std::cout << exp_s.what()<<std::endl;
    };
    /*could not be reach, Illegal instruction: 4 */
    std::cout <<"fuck main again 3" <<std::endl;
    return 0;
}