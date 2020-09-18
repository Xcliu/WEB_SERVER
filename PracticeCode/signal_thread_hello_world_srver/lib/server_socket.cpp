#include "server_socket.h"
#include "socket_address.h"
#include "socket_exception.h"
#include <cstring>
#include <algorithm>

#include <iostream>
using namespace SONNIE;

server_socket::server_socket(
        decltype(AF_INET) _ip_version,
        int _client_num
    ):ip_version(_ip_version),backlog(_client_num)
{
    
    server_socket_fd=-1;
    server_connected_socket_fd=-1;
    
    server_socket_info_ipv4=nullptr;
    client_socket_info_ipv4=nullptr;

    server_socket_info_ipv6=nullptr;
    client_socket_info_ipv6=nullptr;
    //std::cout << "fuck lib"<< std::endl;

    max_buffer_size=1024;// default size 1KB;
    recvd_mesg_buffer="";
    send_mesg_buffer="";
}

server_socket::~server_socket(){
    if(close(server_socket_fd)==-1){
        throw socket_exception("failed to close server socket");
    }
    delete server_socket_info_ipv4;
    delete server_socket_info_ipv6;
    delete client_socket_info_ipv4;
    delete client_socket_info_ipv6;
}

void server_socket::create_socket(decltype(SOCK_STREAM) sock_type){
    //another scoket is SOCK_DGRAM (based on UDP)
    //the operations on TCP and udp are totally different
    server_socket_fd=socket(ip_version,sock_type,0);
    if(server_socket_fd<0){
        throw socket_exception("failed to create a socket");
    }
    return ;
}

void server_socket::bind_socket_to_ipv4_port(
            decltype(INADDR_ANY) _ipv4_address,
            int _server_port
        )
{
    if(ip_version!=AF_INET){
        throw socket_exception("non_ipv4_socket can not be binded to ipv4 address!");
    }
    server_socket_info_ipv4=new socket_info_addr_ipv4(_ipv4_address,_server_port);
    const sockaddr_in *temp=server_socket_info_ipv4->get_sockaddr();
    socklen_t socket_size=sizeof(*temp);
    int result=bind(
        server_socket_fd, 
        (struct sockaddr*)temp,
        socket_size 
    );
    if(result<0){
        throw socket_exception("failed to bind socket!");
    }
    return ;
}

void server_socket::bind_socket_to_ipv6_port(
            uint8_t _ipv6_address[16],
            int _server_port
        )
{
    if(ip_version!=AF_INET6){
        throw socket_exception("non_ipv6_socket can not be binded to ipv6 address!");
    }
    server_socket_info_ipv6=new socket_info_addr_ipv6(_ipv6_address,_server_port);
    const sockaddr_in6 *temp=server_socket_info_ipv6->get_sockaddr();
    socklen_t socket_size=sizeof(*temp);
    int result=bind(
        server_socket_fd, 
        (struct sockaddr*)temp,
        socket_size
    );
    if(result<0){
        throw socket_exception("failed to bind socket!");
    }
    return ;
}

void server_socket::listen_ip_port(){
    int result=listen(server_socket_fd,backlog);
    if(result<0){
        throw socket_exception("failed to listen socket!");
    }
    return ;
}

void server_socket::accept_client_request(){
    socklen_t client_addrLength;
    if(ip_version==AF_INET){
        client_socket_info_ipv4=new socket_info_addr_ipv4();
        sockaddr_in *temp=client_socket_info_ipv4->get_sockaddr();
        client_addrLength=sizeof(*temp); //the size before casting 
        server_connected_socket_fd=accept(server_socket_fd,(struct sockaddr*)temp,&client_addrLength);
        if(server_connected_socket_fd==-1){
            throw socket_exception("failed to accept!");
        }
    }else if(ip_version==AF_INET6){
        client_socket_info_ipv6=new socket_info_addr_ipv6();
        sockaddr_in6 *temp=client_socket_info_ipv6->get_sockaddr();
        client_addrLength=sizeof(*temp);
        server_connected_socket_fd=accept(server_socket_fd,(struct sockaddr*)temp,&client_addrLength);
        if(server_connected_socket_fd==-1){
            throw socket_exception("failed to accept!");
        }
        
    }
    return ;
}

const std::string &server_socket::receive_data_from_client(){
    char temp[max_buffer_size];
    std::fill(temp,temp+max_buffer_size,'\0');
    int result=recv(server_connected_socket_fd,temp,max_buffer_size,0);
    //the size of recvd_msg_buffer is equal to result,(the size of received msg)
    recvd_mesg_buffer=temp;
    if(recvd_mesg_buffer.size()==max_buffer_size){
        throw socket_exception("the receive buffer is overflowed");
    }
    return recvd_mesg_buffer;
}

void server_socket::send_short_mag(const std::string &str){
    //char buf[520]="HTTP/1.1 200 ok\r\nconnection: close\r\n\r\n";//HTTP响应
    if (str.size()>=max_buffer_size){
        throw socket_exception("the size of message is largeer than current buffer");
    }
    int s = send(server_connected_socket_fd,str.c_str(),str.size(),0);//发送响应
    if (s<0){
        throw socket_exception("failed to send short meaasge");
    }
    send_mesg_buffer=str;
    return ;
}

void server_socket::send_file_to_client(const int file_fd, int size)const{
    /*
    *the size of file should be checked.
    */
    if(size<0){
        throw socket_exception("the szie of sent file should be positive");
    }
    off_t send_size=size; //in byte;
    struct sf_hdtr obj{nullptr,0,nullptr,0}; //local scope下不会做默认初始化的！！！
    int temp=sendfile(file_fd,server_connected_socket_fd,0,&send_size,&obj,0);//零拷贝发送消息体
    if(temp==-1){
        throw socket_exception("failed to send file.");
    }
    return ;
}


void server_socket::close_connect_socket(){
    if(close(server_connected_socket_fd)==-1){
        throw socket_exception("failed to close connected scoket");
    }
}

int server_socket::get_connect_fd()const{
    return server_connected_socket_fd;
}

void server_socket::set_msg_buffer_size(size_t size){
    max_buffer_size=size;
}