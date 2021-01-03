#ifndef SERVER_SOCKET_DATAGRAM_H_
#define SERVER_SOCKET_DATAGRAM_H_

#include "socket_common_define.h"
#include "socket_address.h"
#include "server_socket.h"
#include <string>

namespace SONNIE{

class server_socket_datagram:public server_socket
{        
    public: 
        server_socket_datagram(
            decltype(AF_INET) _ip_version=AF_INET
        );
        virtual ~server_socket_datagram();
        virtual const std::string &receive_data_from_client(bool save_client_addr);
        virtual void send_short_mesg(const std::string &str);
        void set_client_addr_ipv4(
            const char *_ipv4_address,
            int _server_port
        );
        void set_client_addr_ipv6(
            const char *_ipv6_address,
            int _server_port
        );
};

} //end of the namespace sonnie


#endif