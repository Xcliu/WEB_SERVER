#include "server_socket.h"
#include "socket_address.h"
#include "socket_exception.h"
#include <cstring>

using namespace SONNIE;

server_socket::server_socket(
        decltype(AF_INET) _ip_version=AF_INET,
        int _client_num=1
    ):ip_version(_ip_version),backlog(_client_num)
{
    server_socket_fd=-1;
    server_connected_socket_fd=-1;
    max_buffer_size=0;
    server_socket_info_ipv4=nullptr;
    client_socket_info_ipv4=nullptr;
    recvd_mesg_buffer=nullptr;
    send_mesg_buffer=nullptr;

}

int server_socket::create_socket(decltype(SOCK_STREAM) sock_type){
    //another scoket is SOCK_DGRAM (based on UDP)
    server_socket_fd=socket(ip_version,sock_type,0);
    if(server_socket_fd<0){
        throw socket_exception("failed to create a socket");
    }
    return server_socket_fd;
}

int server_socket::bind_socket_to_ipv4_port(
            decltype(INADDR_ANY) _ipv4_address=INADDR_ANY,
            int _server_port=8888
        )
{
    if(ip_version!=AF_INET){
        throw socket_exception("non_ipv4_socket can not be binded to ipv4 address!");
    }
    server_socket_info_ipv4=new socket_info_addr_ipv4(_ipv4_address,_server_port);
    const sockaddr_in *temp=server_socket_info_ipv4->get_sockaddr();
    int result=bind(
        server_socket_fd, 
        (struct sockaddr*)temp,
        sizeof(*temp) 
    );
    if(result<0){
        throw socket_exception("failed to bind socket!");
    }
    return result;
}

int server_socket::bind_socket_to_ipv6_port(
            uint8_t _ipv6_address[16]={0},
            int _server_port=8888
        )
{
    if(ip_version!=AF_INET6){
        throw socket_exception("non_ipv6_socket can not be binded to ipv6 address!");
    }
    server_socket_info_ipv6=new socket_info_addr_ipv6(_ipv6_address,_server_port);
    const sockaddr_in6 *temp=server_socket_info_ipv6->get_sockaddr();
    int result=bind(
        server_socket_fd, 
        (struct sockaddr*)temp,
        sizeof(*temp)
    );
    if(result<0){
        throw socket_exception("failed to bind socket!");
    }
    return result;
}

int server_socket::listen_ip_port(){
    int result=listen(server_socket_fd,backlog);
    if(result<0){
        throw socket_exception("failed to listen socket!");
    }
}

int server_socket::accept_client_request(){
    socklen_t client_addrLength;
    if(ip_version==AF_INET){
        client_socket_info_ipv4=new socket_info_addr_ipv4();
        sockaddr_in *temp=client_socket_info_ipv4->get_sockaddr();
        client_addrLength=sizeof(*temp);
        server_connected_socket_fd=accept(server_socket_fd,(struct sockaddr*)temp,&client_addrLength);
        if(server_connected_socket_fd==-1){
            throw socket_exception("failed to accept!");
        }
        return server_socket_fd;
    }else if(ip_version==AF_INET6){
        client_socket_info_ipv6=new socket_info_addr_ipv6();
        sockaddr_in6 *temp=client_socket_info_ipv6->get_sockaddr();
        client_addrLength=sizeof(*temp);
        server_connected_socket_fd=accept(server_socket_fd,(struct sockaddr*)temp,&client_addrLength);
        if(server_connected_socket_fd==-1){
            throw socket_exception("failed to accept!");
        }
        return server_socket_fd;
    }
}

int server_socket::receive_data_from_client(){
    char temp[max_buffer_size]{'\0'};
    int result=recv(server_connected_socket_fd,temp,max_buffer_size,0);
    recvd_mesg_buffer=temp;
    return result;
}

 int server_socket::send_short_mag(std::string str){
    //char buf[520]="HTTP/1.1 200 ok\r\nconnection: close\r\n\r\n";//HTTP响应
    int s = send(server_connected_socket_fd,str.c_str(),str.size(),0);//发送响应
    send_mesg_buffer=str;
    return s;
}

int server_socket::send_file_to_client(const int file_fd, int size)const{
    if(size<0){
        throw socket_exception("the szie of sent file should be positive");
    }
    off_t send_size=size; //in byte;
    struct sf_hdtr obj;
    int temp=-1;
    temp=sendfile(file_fd,server_connected_socket_fd,0,&send_size,&obj,0);//零拷贝发送消息体
    if(temp<0){
        throw socket_exception("failed to send file.");
    }
    return temp;
}

server_socket::~server_socket(){
    delete server_socket_info_ipv4;
    delete server_socket_info_ipv6;
    delete client_socket_info_ipv4;
    delete client_socket_info_ipv6;
}

int server_socket::close_socket(){
    close(server_connected_socket_fd);
}