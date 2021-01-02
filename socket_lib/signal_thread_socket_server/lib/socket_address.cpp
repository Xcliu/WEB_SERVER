#include "socket_address.h"
#include "socket_exception.h"
#include <new>
#include <iostream>
using namespace SONNIE;

socket_info_addr_ipv4::socket_info_addr_ipv4(
            decltype(INADDR_ANY) _ipv4_address,
            int _server_port)
{   
    try{
        addr_ptr=new sockaddr_in();
        dest=new in_addr();
    }catch(const std::bad_alloc &_bad_alloc){
        addr_ptr=nullptr;
        dest=nullptr;
        code_position_info();
        std::cout<<_bad_alloc.what()<<std::endl;;
    }
    memset(addr_ptr,0,sizeof(*addr_ptr));
    addr_ptr->sin_family=AF_INET;
    /*
    *htonl used for 32bits, but htons should be used here?
    */
    addr_ptr->sin_addr.s_addr=htons(_ipv4_address);
    addr_ptr->sin_port=htons(_server_port);
};

socket_info_addr_ipv4::~socket_info_addr_ipv4(){
    delete addr_ptr;
    delete dest;
}

sockaddr_in *socket_info_addr_ipv4::get_sockaddr()const{
    return addr_ptr;
}

void socket_info_addr_ipv4::set_ip_port_ipv4(
        const char *_ipv4_address,
        int _server_port
    )
{
    memset(dest,0,sizeof(*dest));
    int temp=inet_pton(AF_INET,_ipv4_address,dest);
    if(temp<0){
        systemcall_error_info();
        throw socket_exception("faile to transform ipv4 address");
    }
    addr_ptr->sin_addr=*dest;
    addr_ptr->sin_port=htons(_server_port); 
}

socket_info_addr_ipv6::socket_info_addr_ipv6(
        in6_addr _ipv6_address,
        int _server_port
    )
{
    try{
        addr_ptr=new sockaddr_in6();
        dest=new in6_addr();
    }catch(const std::bad_alloc & _bad_alloc){
        addr_ptr=nullptr;
        dest=nullptr;
        code_position_info();
        std::cout<<_bad_alloc.what()<<std::endl; 
    }
    memset(addr_ptr,0,sizeof(*addr_ptr));
    /*
    *the dendianess problem does not exist for array(stackoverflow).?
    */
    addr_ptr->sin6_addr=_ipv6_address;
    addr_ptr->sin6_family=AF_INET6;
    addr_ptr->sin6_port=htons(_server_port);
};

socket_info_addr_ipv6::~socket_info_addr_ipv6(){
    delete addr_ptr;
    delete dest;
}

sockaddr_in6 *socket_info_addr_ipv6::get_sockaddr()const{
    return addr_ptr;
}

void socket_info_addr_ipv6::set_ip_port_ipv6(
        const char *_ipv6_address,
        int _server_port
    )
{
    memset(dest,0,sizeof(*dest));
    int temp=inet_pton(AF_INET6,_ipv6_address,dest);
    if(temp<0){
        systemcall_error_info();
        throw socket_exception("faile to transform ipv6 address");
    }
    addr_ptr->sin6_addr=*dest;
    addr_ptr->sin6_port=htons(_server_port); 
}