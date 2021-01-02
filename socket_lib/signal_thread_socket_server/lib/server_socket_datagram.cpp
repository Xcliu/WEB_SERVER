#include "server_socket.h"
#include "server_socket_datagram.h"
#include "socket_address.h"
#include "socket_exception.h"
#include <cstring>
#include <algorithm>

#include <iostream>
using namespace SONNIE;

server_socket_datagram::server_socket_datagram(
        decltype(AF_INET) _ip_version
    ):server_socket(_ip_version,SOCK_DGRAM)
{
    if(ip_version==AF_INET){
        try{
            client_socket_info_ipv4=new socket_info_addr_ipv4();
        }catch(const std::bad_alloc &_bad_alloc){
            client_socket_info_ipv4=nullptr;
            code_position_info();
            std::cout<<_bad_alloc.what()<<std::endl;;
        }
    }else if(ip_version==AF_INET6){
        try{
            client_socket_info_ipv6=new socket_info_addr_ipv6();
        }catch(const std::bad_alloc &_bad_alloc){
            client_socket_info_ipv6=nullptr;
            code_position_info();
            std::cout<<_bad_alloc.what()<<std::endl;;
        }
    }
}

server_socket_datagram::~server_socket_datagram(){
   server_socket::~server_socket();
}

const std::string &server_socket_datagram::receive_data_from_client(bool save_client_addr){
    char temp[max_buffer_size];
    std::fill(temp,temp+max_buffer_size,'\0');

    socklen_t ipv4_len=sizeof(*(client_socket_info_ipv4->get_sockaddr()));
    socklen_t ipv6_len=sizeof(*(client_socket_info_ipv6->get_sockaddr()));
    socklen_t src_len=ip_version==AF_INET?ipv4_len:ipv6_len;
    
    sockaddr  *src_ptr=nullptr;
    if(save_client_addr){       
        sockaddr *src_ptr=ip_version==AF_INET?(sockaddr *)(client_socket_info_ipv4->get_sockaddr()):
            (sockaddr *)(client_socket_info_ipv6->get_sockaddr());
    }
    ssize_t mesg_len=recvfrom(server_socket_fd,temp,max_buffer_size,0,src_ptr,&src_len);
    //the size of recvd_msg_buffer is equal to result,(the size of received msg)
    if(mesg_len==max_buffer_size){
        throw socket_exception("the receive buffer is overflowed");
    }else if(mesg_len<0){
        systemcall_error_info();
        throw socket_exception("failed to receive message");
    }
    recvd_mesg_buffer=temp;
    return recvd_mesg_buffer;
}

void server_socket_datagram::send_short_mesg(const std::string &str){
    //char buf[520]="HTTP/1.1 200 ok\r\nconnection: close\r\n\r\n";//HTTP响应
    if (str.size()>=max_buffer_size){
        throw socket_exception("the size of message is largeer than current buffer");
    }
    socklen_t ipv4_len=sizeof(*(client_socket_info_ipv4->get_sockaddr()));
    socklen_t ipv6_len=sizeof(*(client_socket_info_ipv6->get_sockaddr()));
    socklen_t dest_len=ip_version==AF_INET?ipv4_len:ipv6_len;
    
    sockaddr *dest_ptr=ip_version==AF_INET?(sockaddr *)(client_socket_info_ipv4->get_sockaddr()):
        (sockaddr *)(client_socket_info_ipv6->get_sockaddr());
        
    ssize_t mesg_len = sendto(server_socket_fd,str.c_str(),str.size(),0,dest_ptr,dest_len);//发送响应
    if(mesg_len==max_buffer_size){
        throw socket_exception("the send buffer is overflowed");
    }else if(mesg_len<0){
        systemcall_error_info();
        throw socket_exception("failed to send message");
    }else if(mesg_len!=0){
        std::cout<<"successfully send! "<<std::endl;
    }
    send_mesg_buffer=str;
    return ;
}

void server_socket_datagram::set_client_addr_ipv4(
           const char *_ipv4_address,
            int _server_port
        )
{
    if(ip_version!=AF_INET){
            throw socket_exception("non_ipv4_socket can not be set to ipv4 address!");
    }
    client_socket_info_ipv4->set_ip_port_ipv4(_ipv4_address,_server_port);
    return ;
}

void server_socket_datagram::set_client_addr_ipv6(
        const char *_ipv6_address,
        int _server_port
    )
{
    if(ip_version!=AF_INET6){
        throw socket_exception("non_ipv6_socket can not be set to ipv6 address!");
    }
    client_socket_info_ipv6->set_ip_port_ipv6(_ipv6_address,_server_port);
    return ;
}

