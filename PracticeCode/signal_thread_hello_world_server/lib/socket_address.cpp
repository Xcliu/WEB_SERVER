#include "socket_address.h"
#include "socket_exception.h"
#include <new>
#include <iostream>
using namespace SONNIE;

socket_info_addr_ipv4::socket_info_addr_ipv4(
            decltype(INADDR_ANY) _ipv4_address,
            int _server_port)
{   try{
        addr_ptr=new sockaddr_in();
    }catch(const std::bad_alloc &_bad_alloc){
        addr_ptr=nullptr;
        code_position_info();
        std::cout<<_bad_alloc.what()<<std::endl;;
    }
    addr_ptr->sin_family=AF_INET;
    /*
    *htonl used for 32bits, but htons should be used here?
    */
    addr_ptr->sin_addr.s_addr=htons(_ipv4_address);
    addr_ptr->sin_port=htons(_server_port);
};

socket_info_addr_ipv4::~socket_info_addr_ipv4(){
    delete addr_ptr;
}

sockaddr_in *socket_info_addr_ipv4::get_sockaddr()const{
    return addr_ptr;
}

void socket_info_addr_ipv4::change_ip_port_ipv4(
        decltype(INADDR_ANY) _ipv4_address,
        int _server_port
    )
{
    addr_ptr->sin_addr.s_addr==htons(_ipv4_address);
    addr_ptr->sin_port=htons(_server_port);    
}

socket_info_addr_ipv6::socket_info_addr_ipv6(
        uint8_t _ipv6_address[16],
        int _server_port
    )
{
    try{
        addr_ptr=new sockaddr_in6();
    }catch(const std::bad_alloc & _bad_alloc){
        addr_ptr=nullptr;
        code_position_info();
        std::cout<<_bad_alloc.what()<<std::endl; 
    }
    /*
    *the dendianess problem does not exist for array(stackoverflow).?
    */
    for(int i=0;i<16;++i){
        addr_ptr->sin6_addr.s6_addr[i]=_ipv6_address[i];
    }
    addr_ptr->sin6_family=AF_INET6;
    addr_ptr->sin6_port=htons(_server_port);
};

socket_info_addr_ipv6::~socket_info_addr_ipv6(){
    delete addr_ptr;
}

sockaddr_in6 *socket_info_addr_ipv6::get_sockaddr()const{
    return addr_ptr;
}

void socket_info_addr_ipv6::change_ip_port_ipv6(
        uint8_t _ipv6_address[16],
        int _server_port
    )
{
    for(int i=0;i<16;++i){
        addr_ptr->sin6_addr.s6_addr[i]=_ipv6_address[i];
    }
    addr_ptr->sin6_port=htons(_server_port);
}