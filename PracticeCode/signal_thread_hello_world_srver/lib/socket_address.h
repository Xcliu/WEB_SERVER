#ifndef SOCKET_ADDRESS_H_
#define SOCKET_ADDRESS_H_

#include "socket_related_head_file.h"

namespace SONNIE{
class socket_info_addr_ipv4
{
    private:
        sockaddr_in *addr_ptr;
    public:
        socket_info_addr_ipv4(
            decltype(INADDR_ANY) _ipv4_address=INADDR_ANY,
            int _server_port=8888
        );
        ~socket_info_addr_ipv4();
        sockaddr_in* get_sockaddr()const;
};


class socket_info_addr_ipv6
{
    private:
        sockaddr_in6 *addr_ptr;
        // const decltype(AF_INET6) ip_version;
        // uint8_t ipv6_address[16];
        // const int server_port;

    public:
        socket_info_addr_ipv6(
            uint8_t _ipv6_address[16]={0},
            int _server_port=8888
        );
        ~socket_info_addr_ipv6();
        sockaddr_in6* get_sockaddr()const;

};

}
#endif 