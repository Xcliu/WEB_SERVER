#include "WEB_SERVER/socket_lib/server_socket_stream.h"
#include "WEB_SERVER/socket_lib/socket_exception.h"
#include <iostream>
#include <string>
using namespace SONNIE; 

#define html_path \
    "/root/lxc/git_project/WEB_SERVER/unit_test/test_socket/test_stream_socket/hello.html"

int main(){
    std::cout << "hello world WEB server!" <<std::endl;
    try{
        server_socket_stream server_socket_obj(AF_INET,1);
        server_socket_obj.init_socket();
        server_socket_obj.bind_socket_to_ipv4_port("127.0.0.1",8888);
        server_socket_obj.listen_ip_port();
        while(1){
            server_socket_obj.accept_client_request();
            std::string received_mesg=server_socket_obj.receive_data_from_client(1);
            std::cout<< "received_mesg: " << received_mesg <<std::endl;
            std::string str_send="HTTP/1.1 200 ok\r\nconnection: close\r\n\r\n";
            server_socket_obj.send_short_mesg(str_send);
            int fd_html=open(html_path,O_RDONLY);
            server_socket_obj.send_file_to_client(fd_html, 1024);
            close(fd_html);
            server_socket_obj.close_connect_socket();
        }
    }catch (socket_exception &exp){
            std::cout << exp.what()<<std::endl;
    };
    return 0;
}


