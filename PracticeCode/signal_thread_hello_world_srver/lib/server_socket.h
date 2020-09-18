#ifndef SERVER_SOCKET_H_
#define SERVER_SOCKET_H_

#include "socket_related_head_file.h"
#include "socket_address.h"
#include <string>


namespace SONNIE{

class server_socket
{
    private:
        const int backlog; // how many clients cloud be blocked in one server socket 
        const decltype(AF_INET) ip_version;
        
        int server_socket_fd;
        //int server_connected_socket_fd;
        int server_connected_socket_fd;
        
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
            int _client_num=1
        );
        ~server_socket();
        void create_socket(decltype(SOCK_STREAM) sock_type);
        void bind_socket_to_ipv4_port(
            decltype(INADDR_ANY) _ipv4_address=INADDR_ANY,
            int _server_port=8888
        );
        void bind_socket_to_ipv6_port(
            uint8_t _ipv6_address[16]={0},
            int _server_port=8888
        );
        void listen_ip_port();
        void accept_client_request();
        const std::string &receive_data_from_client();
        void send_short_mag(const std::string &str);
        void send_file_to_client(int file_fd, int size)const;
        void close_connect_socket();
        /*
        added in 9.18.2020
        */
        int get_connect_fd()const;
        void set_msg_buffer_size(size_t size);
   
};

} //end of the namespace sonnie
#endif

