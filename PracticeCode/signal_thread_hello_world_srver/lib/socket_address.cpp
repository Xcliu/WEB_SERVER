#include "socket_address.h"

using namespace SONNIE;

socket_info_addr_ipv4::socket_info_addr_ipv4(
            decltype(INADDR_ANY) _ipv4_address,
            int _server_port)
{
    addr_ptr=new sockaddr_in();
    addr_ptr->sin_family=AF_INET;
    addr_ptr->sin_addr.s_addr=htons(_ipv4_address);
    addr_ptr->sin_port=htons(_server_port);
};

socket_info_addr_ipv4::~socket_info_addr_ipv4(){
    delete addr_ptr;
}

sockaddr_in *socket_info_addr_ipv4::get_sockaddr()const{
    return addr_ptr;
}


socket_info_addr_ipv6::socket_info_addr_ipv6(
        uint8_t _ipv6_address[16],
        int _server_port
    )
{
    addr_ptr=new sockaddr_in6();
    uint8_t ipv6_address_temp[16];
    for(int i=0;i<16;++i){
        ipv6_address_temp[i]=_ipv6_address[i];
    }
    /*
    *!need check!
    *not sure about the network byte oreder 
    *is right for ipv6 address 
    */
    for(int i=15;i<=0;--i){
        addr_ptr->sin6_addr.s6_addr[15-i]=htons(ipv6_address_temp[i]);
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