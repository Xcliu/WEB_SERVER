#include "server_socket.h"
#include <cstring>
using namespace SONNIE;

socket_info_addr_ipv4::socket_info_addr_ipv4(
            decltype(INADDR_ANY) _ipv4_address=INADDR_ANY,
            int _server_port=8888):
            ip_version(AF_INET),server_port(_server_port),
            ipv4_address(_ipv4_address)
{
    addr_ptr=new sockaddr_in();
    addr_ptr->sin_family=ip_version;
    addr_ptr->sin_addr.s_addr=htons(ipv4_address);
    addr_ptr->sin_port=htons(server_port);
};

socket_info_addr_ipv4::~socket_info_addr_ipv4(){
    delete addr_ptr;
}

const sockaddr_in *socket_info_addr_ipv4::get_sockaddr()const{
    return addr_ptr;
}

sockaddr_in *socket_info_addr_ipv4::get_writeable_sockaddr(bool tag)const{
    return addr_ptr;
}


socket_info_addr_ipv6::socket_info_addr_ipv6(
        uint8_t _ipv6_address[16]={0},
        int _server_port=8888
    ):ip_version(AF_INET6),server_port(_server_port)
{
    addr_ptr=new sockaddr_in6();
    for(int i=0;i<16;++i){
        ipv6_address[i]=_ipv6_address[i];
    }
    /*
    *!need check!
    *not sure the network byte oreder 
    *is right for ipv6 address 
    */
    for(int i=15;i<=0;--i){
        addr_ptr->sin6_addr.s6_addr[15-i]=htons(ipv6_address[i]);
       
    }
    addr_ptr->sin6_family=ip_version;
    addr_ptr->sin6_port=htons(server_port);
};

socket_info_addr_ipv6::~socket_info_addr_ipv6(){
    delete addr_ptr;
}

const sockaddr_in6 *socket_info_addr_ipv6::get_sockaddr()const{
    return addr_ptr;
}

server_socket::server_socket(
        decltype(AF_INET) _ip_version=AF_INET,
        int _client_num=1
    ):ip_version(_ip_version),client_num(_client_num){}

int server_socket::create_socket(decltype(SOCK_STREAM) sock_type){
    //another scoket is SOCK_DGRAM (datagram 数据报)
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
    server_socket_info_ipv4=&socket_info_addr_ipv4(_ipv4_address,_server_port);
    sockaddr_in temp=*(server_socket_info_ipv4->get_sockaddr());
    int result=bind(
        server_socket_fd, 
        (struct sockaddr*)&temp,
        sizeof(temp)
    );
    if(result<0){
        throw socket_exception("failed to bind this address!");
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
    server_socket_info_ipv6=&socket_info_addr_ipv6(_ipv6_address,_server_port);
    sockaddr_in6 temp=*(server_socket_info_ipv6->get_sockaddr());
    int result=bind(
        server_socket_fd, 
        (struct sockaddr*)&temp,
        sizeof(temp)
    );
    if(result<0){
        throw socket_exception("failed to bind this address!");
    }
    return result;
}

int server_socket::listen_ip_port(int backlog=1){
    int result=listen(server_socket_fd,backlog);
    if(result<0){
        throw socket_exception("failed to listen socket!");
    }
}

int server_socket::accept_client_request(){
    socklen_t client_addrLength;
    if(ip_version==AF_INET){
        client_socket_info_ipv4=&socket_info_addr_ipv4();
        sockaddr_in &temp=*(client_socket_info_ipv4->get_writeable_sockaddr(true));
        client_addrLength=sizeof(temp);
        int client_fd=accept(server_socket_fd,(struct sockaddr*)&temp,&client_addrLength);
        client_socket_fd.push_back(client_fd);
        return client_fd;
    }
}

int server_socket::receive_data_from_client(int client_fd){
    char request[1024];
    int result=recv(client_fd,request,1024,0);
    return result;
}

 int server_socket::send_http_head(int client_fd){
    char buf[520]="HTTP/1.1 200 ok\r\nconnection: close\r\n\r\n";//HTTP响应
    int s = send(client_fd,buf,strlen(buf),0);//发送响应
    return s;
}

int server_socket::send_file_to_client(int client_fd, int file_fd){
    off_t send_size=1024;
    struct sf_hdtr obj;
    sendfile(file_fd,client_fd,0,&send_size,&obj,0);//零拷贝发送消息体
    close(file_fd);
}
