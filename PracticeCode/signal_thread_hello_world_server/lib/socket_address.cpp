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
        const char *_ipv4_address,
        int _server_port
    )
{
    //std::cout<< _ipv4_address<<";"<<_server_port<<std::endl;
    // delete addr_ptr;
    // addr_ptr=new sockaddr_in();
    //addr_ptr->sin_family=AF_INET;
    
    in_addr *dest=new in_addr{0};
    inet_pton(AF_INET,_ipv4_address,dest);
    addr_ptr->sin_addr=*dest;
    //std::cout<<"lib1: "<<(addr_ptr->sin_addr.s_addr)<<std::endl;
    
    addr_ptr->sin_port=htons(_server_port); 

    // in_addr *dest=new in_addr{0};
    // sockaddr_in siTo;
    // siTo.sin_family = AF_INET;
    // siTo.sin_port = htons(9527);
    // inet_pton(AF_INET,"127.0.0.1",dest);
    // //siTo.sin_addr=*dest;//*dest++;
    // std::cout<<"dd: "<<siTo.sin_addr.s_addr<<std::endl;

}

socket_info_addr_ipv6::socket_info_addr_ipv6(
        struct in6_addr _ipv6_address,
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

    addr_ptr->sin6_addr=_ipv6_address;
    //  std::cout << "add_lib" <<std::endl; 
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