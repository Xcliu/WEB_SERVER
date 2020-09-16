#include "server_socket.h"
#include "socket_related_head_file.h"
#include "socket_exception.h"
#include <iostream>
#include <string>
using namespace SONNIE; 

int main(){
    std::cout << "hello world WEB server!" <<std::endl;
    try{
        server_socket server_socket_obj(AF_INET,1);
        server_socket_obj.create_socket(SOCK_STREAM);
        server_socket_obj.bind_socket_to_ipv4_port();
        server_socket_obj.listen_ip_port();
        std::cout << "fuck main"<< std::endl;
    
        while(1){
            int connected_fd=server_socket_obj.accept_client_request();
            server_socket_obj.receive_data_from_client();
            std::string received_mag=server_socket_obj.recvd_mesg_buffer;
            std::string str_send="HTTP/1.1 200 ok\r\nconnection: close\r\n\r\n";
            server_socket_obj.send_short_mag(str_send);
            int fd_html=open("hello.html",O_RDONLY);
            std::cout << fd_html <<std::endl; 
            server_socket_obj.send_file_to_client(fd_html, 1024);
            close(fd_html);
            server_socket_obj.close_socket();
        }
    }catch (socket_exception &exp){
            std::cout << exp.what()<<std::endl;
    };
    return 0;
}


