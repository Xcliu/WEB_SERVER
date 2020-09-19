#ifndef SERVER_SOCKET_DATAGRAM_H_
#define SERVER_SOCKET_DATAGRAM_H_

#include "socket_related_head_file.h"
#include "socket_address.h"
#include "server_socket.h"

#include <string>

namespace SONNIE{

class server_socket_datagram:public server_socket
{
    private:
        // how many clients cloud be blocked in one server socket 
        
    public: 
        server_socket_datagram(
            decltype(AF_INET) _ip_version=AF_INET
        );
        virtual ~server_socket_datagram();
        virtual const std::string &receive_data_from_client(bool save_client_addr);
        virtual void send_short_mesg(const std::string &str);
        void set_client_addr_ipv4(
            decltype(INADDR_ANY) _ipv4_address=INADDR_ANY,
            int _server_port=8888
        );
        void set_client_addr_ipv6(
            uint8_t _ipv6_address[16]={0},
            int _server_port=8888
        );
   
};

} //end of the namespace sonnie


#endif