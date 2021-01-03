#include "server_socket.h"
#include "server_socket_stream.h"
#include "socket_address.h"
#include "socket_exception.h"
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace SONNIE;

server_socket_stream::server_socket_stream(
        decltype(AF_INET) _ip_version,
        int _client_num
    ):server_socket(_ip_version,SOCK_STREAM),backlog(_client_num)
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
    server_connected_socket_fd=-1;
}

server_socket_stream::~server_socket_stream(){
   server_socket::~server_socket();
}

void server_socket_stream::listen_ip_port(){
    int result=listen(server_socket_fd,backlog);
    if(result<0){
        throw socket_exception("failed to listen socket!");
    }
    return ;
}

void server_socket_stream::accept_client_request(){
    socklen_t client_addrLength;
    if(ip_version==AF_INET){
        sockaddr_in *temp=client_socket_info_ipv4->get_sockaddr();
        client_addrLength=sizeof(*temp); //the size before casting 
        // std::cout<< "fuck lib"<<std::endl;
        server_connected_socket_fd=accept(server_socket_fd,(struct sockaddr*)temp,&client_addrLength);
        if(server_connected_socket_fd==-1){
            systemcall_error_info();
            throw socket_exception("failed to accept!");
        }
    }else if(ip_version==AF_INET6){
        sockaddr_in6 *temp=client_socket_info_ipv6->get_sockaddr();
        client_addrLength=sizeof(*temp);
        server_connected_socket_fd=accept(server_socket_fd,(struct sockaddr*)temp,&client_addrLength);
        if(server_connected_socket_fd==-1){
            systemcall_error_info();
            throw socket_exception("failed to accept!");
        }
    }
    return ;
}

const std::string &server_socket_stream::receive_data_from_client(bool){
    char temp[max_buffer_size];
    std::fill(temp,temp+max_buffer_size,'\0');
    int result=recv(server_connected_socket_fd,temp,max_buffer_size,0);
    //the size of recvd_msg_buffer is equal to result,(the size of received msg)
    if(recvd_mesg_buffer.size()==max_buffer_size){
        throw socket_exception("the receive buffer is overflowed");
    }else if(recvd_mesg_buffer.size()<0){
        systemcall_error_info();
        throw socket_exception("fail to receive messages");
    }
    recvd_mesg_buffer=temp;
    return recvd_mesg_buffer;
}

void server_socket_stream::send_short_mesg(const std::string &str){
    //char buf[520]="HTTP/1.1 200 ok\r\nconnection: close\r\n\r\n";//HTTP响应
    if (str.size()>=max_buffer_size){
        throw socket_exception("the size of message is largeer than current buffer");
    }
    int s = send(server_connected_socket_fd,str.c_str(),str.size(),0);//发送响应
    if (s<0){
        systemcall_error_info();
        throw socket_exception("failed to send short meaasge");
    }
    send_mesg_buffer=str;
    return ;
}

void server_socket_stream::send_file_to_client(const int file_fd, int size)const{
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
        systemcall_error_info();
        throw socket_exception("failed to send file.");
    }
    return ;
}


void server_socket_stream::close_connect_socket(){
    if(close(server_connected_socket_fd)==-1){
        systemcall_error_info();
        throw socket_exception("failed to close connected scoket");
    }
}

int server_socket_stream::get_connect_fd()const{
    return server_connected_socket_fd;
}



