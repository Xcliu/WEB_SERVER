#ifndef SERVER_SOCKET_STREAM_H_
#define SERVER_SOCKET_STREAM_H_

#include "socket_common_define.h"
#include "socket_address.h"
#include "server_socket.h"

#include <string>


namespace SONNIE{

class server_socket_stream:public server_socket
{
    private:
        // how many clients cloud be blocked in one server socket 
        const int backlog; 
        int server_connected_socket_fd;
    public: 
        server_socket_stream(
            decltype(AF_INET) _ip_version=AF_INET,
            int _client_num=1
        );
        virtual ~server_socket_stream();
        void listen_ip_port();
        void accept_client_request();
        virtual const std::string &receive_data_from_client(bool);
        virtual void send_short_mesg(const std::string &str);
        void send_file_to_client(int file_fd, int size)const;
        void close_connect_socket();
        int get_connect_fd()const;
        std::string get_client_info_ipv4()const;
        std::string get_client_info_ipv6()const;
   
};

} //end of the namespace sonnie
#endif

