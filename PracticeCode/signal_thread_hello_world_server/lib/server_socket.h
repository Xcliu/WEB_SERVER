#ifndef SERVER_SOCKET_H_
#define SERVER_SOCKET_H_

#include "socket_related_head_file.h"
#include "socket_address.h"
#include <string>

namespace SONNIE{

class server_socket{
    protected:
        const decltype(AF_INET) ip_version;
        const decltype(SOCK_STREAM) socket_type;

        int server_socket_fd;
        /*
        * allocating memory in heap
        */
        socket_info_addr_ipv4 *server_socket_info_ipv4;
        socket_info_addr_ipv4 *client_socket_info_ipv4;

        socket_info_addr_ipv6 *server_socket_info_ipv6;
        socket_info_addr_ipv6 *client_socket_info_ipv6;

        size_t max_buffer_size;
        std::string recvd_mesg_buffer;
        std::string send_mesg_buffer;
    public:
        server_socket(
            decltype(AF_INET) _ip_version=AF_INET,
            decltype(SOCK_STREAM) _socket_type=SOCK_STREAM
        );
        virtual ~server_socket();
        void create_socket();
        void bind_socket_to_ipv4_port(
            decltype(INADDR_ANY) _ipv4_address=INADDR_ANY,
            int _server_port=8888
        );
        void bind_socket_to_ipv6_port(
            struct in6_addr _ipv4_address=IN6ADDR_ANY_INIT,
            int _server_port=8888
        );
        virtual const std::string &receive_data_from_client(bool save_client_addr)=0;
        virtual void send_short_mesg(const std::string &str)=0;
        
        void close_server_socket()const;
        void set_mesg_buffer_size(size_t size);      
};
};

#endif