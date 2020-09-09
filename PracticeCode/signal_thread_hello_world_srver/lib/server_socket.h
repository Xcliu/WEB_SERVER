#ifndef SERVER_SOCKET_H_
#define SERVER_SOCKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

/*for mac os*/
#if __APPLE__
    #include <sys/uio.h>
#elif /*for Linux system call*/
    #include <sys/sendfile.h>
#endif

#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
namespace SONNIE{
class socket_info_addr_ipv4
{
    private:
        sockaddr_in *addr_ptr;
        const decltype(AF_INET) ip_version;
        decltype(INADDR_ANY) ipv4_address;
       //uint8_t ipv6_address[16];
        const int server_port;

    public:
        socket_info_addr_ipv4(
            decltype(INADDR_ANY) _ipv4_address=INADDR_ANY,
            int _server_port=8888
        );
        ~socket_info_addr_ipv4();
        const sockaddr_in* get_sockaddr()const;
        sockaddr_in* get_writeable_sockaddr(bool tag)const;
};


class socket_info_addr_ipv6
{
    private:
        sockaddr_in6 *addr_ptr;
        const decltype(AF_INET6) ip_version;
        uint8_t ipv6_address[16];
        const int server_port;

    public:
        socket_info_addr_ipv6(
            uint8_t _ipv6_address[16]={0},
            int _server_port=8888
        );
        ~socket_info_addr_ipv6();
        const sockaddr_in6* get_sockaddr()const;
        sockaddr_in6* get_wrietable_sockaddr(bool tag)const;

};

class server_socket
{
    private:
        const int client_num;
        const decltype(AF_INET) ip_version;
        int server_socket_fd;
        std::vector<int> client_socket_fd;
        socket_info_addr_ipv4 *server_socket_info_ipv4;
        socket_info_addr_ipv6 *server_socket_info_ipv6;
        socket_info_addr_ipv4 *client_socket_info_ipv4;
        socket_info_addr_ipv6 *client_socket_info_ipv6;
    public: 
        server_socket(
            decltype(AF_INET) _ip_version=AF_INET,
            int _client_num=1
        );
        int create_socket(decltype(SOCK_STREAM) sock_type);
        int bind_socket_to_ipv4_port(
            decltype(INADDR_ANY) _ipv4_address=INADDR_ANY,
            int _server_port=8888
        );
        int bind_socket_to_ipv6_port(
            uint8_t _ipv6_address[16]={0},
            int _server_port=8888
        );
        int listen_ip_port(int backlog=1);
        int accept_client_request();
        int receive_data_from_client(int client_fd);
        int send_http_head(int client_fd);
        int send_file_to_client(int client_fd,int file_fd);
        int close_socket();
};

} //end of the namespace sonnie
#endif

