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
        client_socket_info_ipv4=new socket_info_addr_ipv4();
    }else if(ip_version==AF_INET6){
        client_socket_info_ipv6=new socket_info_addr_ipv6();
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

    sockaddr *dest_ptr=ip_version==AF_INET?(sockaddr *)(client_socket_info_ipv4->get_sockaddr()):
    (sockaddr *)(client_socket_info_ipv6->get_sockaddr());
    
    sockaddr  *src_ptr=nullptr;
    if(save_client_addr){       
        sockaddr *src_ptr=ip_version==AF_INET?(sockaddr *)(client_socket_info_ipv4->get_sockaddr()):
            (sockaddr *)(client_socket_info_ipv6->get_sockaddr());
    }
    std::cout<<"before receive sysytem call " <<std::endl;
    ssize_t mesg_len=recvfrom(server_socket_fd,temp,max_buffer_size,0,src_ptr,&src_len);
    //the size of recvd_msg_buffer is equal to result,(the size of received msg)
    std::cout<<"after receive sysytem call " <<std::endl;
    recvd_mesg_buffer=temp;
    if(mesg_len==max_buffer_size){
        throw socket_exception("the receive buffer is overflowed");
    }else if(mesg_len<0){
        throw socket_exception("failed to receive message");
    }
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
    
    //ipv4
    // std::cout<< server_socket_fd << str <<str.size()<<std::endl;
    // in_addr *dest=new in_addr{0};
    // sockaddr_in siTo;
    // siTo.sin_family = AF_INET;
    // siTo.sin_port = htons(9527);
    // inet_pton(AF_INET,"127.0.0.1",dest);
    // siTo.sin_addr=*dest;//*dest++;
    // std::cout<<"dd: "<<(((sockaddr*)&siTo)->sa_data)<<";"<<(((sockaddr*)&siTo)->sa_family)<<";"<<(((sockaddr*)&siTo)->sa_len)<<std::endl;
    // std::cout<<"arguments: "<<(dest_ptr->sa_data)<<";"<<(dest_ptr->sa_family)<<";"<<(dest_ptr->sa_len)<<std::endl;
    // ssize_t mesg_len = sendto(server_socket_fd,str.c_str(),str.size(),0,(sockaddr*)&siTo,sizeof(siTo));//发送响应
    
    
    //ipv6
    // in6_addr *dest=new in6_addr{0};
    // sockaddr_in6 siTo;
    // siTo.sin6_family = AF_INET6;
    // siTo.sin6_port = htons(9527);
    // int s=inet_pton(AF_INET6,"::1",dest);
    // if (s <= 0) {
    //     if (s == 0)
    //         fprintf(stderr, "Not in presentation format\n");
    //     else
    //         perror("inet_pton");
    //     exit(EXIT_FAILURE);
    // }
    // siTo.sin6_addr=*dest;//*dest++;
    // std::cout<<(siTo.sin6_addr.s6_addr)<<std::endl;
    // \\\ssize_t mesg_len = sendto(server_socket_fd,str.c_str(),str.size(),0,(sockaddr*)&siTo,sizeof(siTo));//发送响应
    ssize_t mesg_len = sendto(server_socket_fd,str.c_str(),str.size(),0,dest_ptr,dest_len);//发送响应
    if(mesg_len==max_buffer_size){
        throw socket_exception("the send buffer is overflowed");
    }else if(mesg_len<0){
        int err = errno;
        fprintf(stderr, "*** ERROR  failed:%d(%s)\n", err, strerror(err) );
        exit(EXIT_FAILURE);
        throw socket_exception("failed to send message");
    }else if(mesg_len!=0){
        std::cout<<"send successfully !"<<std::endl;
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
    client_socket_info_ipv4->change_ip_port_ipv4(_ipv4_address,_server_port);
    return ;
}

void server_socket_datagram::set_client_addr_ipv6(
        uint8_t _ipv6_address[16],
        int _server_port
    )
{
    if(ip_version!=AF_INET6){
        throw socket_exception("non_ipv4_socket can not be set to ipv4 address!");
    }
    client_socket_info_ipv6->change_ip_port_ipv6(_ipv6_address,_server_port);
    return ;
}

