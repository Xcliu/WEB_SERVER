#include "server_socket.h"
#include "socket_exception.h"
using namespace SONNIE;


server_socket::server_socket(
        decltype(AF_INET) _ip_version,
        decltype(SOCK_STREAM) _socket_type
    ):ip_version(_ip_version),socket_type(_socket_type)
{
    server_socket_fd=-1;
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
    delete server_socket_info_ipv4;
    delete server_socket_info_ipv6;
    delete client_socket_info_ipv4;
    delete client_socket_info_ipv6;
}

void server_socket::create_socket(){
    //another scoket is SOCK_DGRAM (based on UDP)
    //the operations on TCP and udp are totally different
    server_socket_fd=socket(ip_version,socket_type,0);
    // std::cout<< server_socket_fd <<std::endl;
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
    delete server_socket_info_ipv4;
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
    delete  server_socket_info_ipv6;
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

void server_socket::close_server_socket()const{
    if(close(server_socket_fd)==-1){
        throw socket_exception("failed to close connected scoket");
    }
}

void server_socket::set_mesg_buffer_size(size_t size){
    max_buffer_size=size;
}