#pragma once

#include <string>
#include <memory>
#include "socket_common_define.h"
#include "socket_address.h"


namespace SONNIE{
    
class server_socket{
    protected:
        const decltype(AF_INET) ip_version_;
        const decltype(SOCK_STREAM) socket_type_;
        int server_socket_fd_ = -1;
        /*
        * allocating memory in heap
        * 
        * used for saving the struct containing ip address and port
        * In TCP mode, sever_*_*_* saving the server info, and used for bind()
        *client_*_*_* used for returning the client info in sysytem call accept()
        *
        * In UDP mode,sever_*_*_* saving the server info, and used for bind()
        * client_*_*_* used for assigning the client address in sysytem call sendto()
        */
        // NOTE(lxc): only two of them are used in one instance
        // allocate memory at init stategy to avoid lock (during malloc)
        std::shared_ptr<socket_info_addr_ipv4> server_socket_info_ipv4_ = 
                                            std::make_shared<socket_info_addr_ipv4>();
        std::shared_ptr<socket_info_addr_ipv6> server_socket_info_ipv6_ =
                                            std::make_shared<socket_info_addr_ipv6>();
        std::shared_ptr<socket_info_addr_ipv4> client_socket_info_ipv4_ = 
                                            std::make_shared<socket_info_addr_ipv4>();
        std::shared_ptr<socket_info_addr_ipv6> client_socket_info_ipv6_ = 
                                            std::make_shared<socket_info_addr_ipv6>();
        size_t max_buffer_size_ = 1024;
        std::string recvd_mesg_buffer_ = "";
        std::string send_mesg_buffer_ = "";
    public:
        server_socket( decltype(AF_INET) ip_version=AF_INET,decltype(SOCK_STREAM) socket_type=SOCK_STREAM)
                    :ip_version_(ip_version),socket_type_(socket_type){}
        virtual ~server_socket();
        server_socket(const server_socket& ) = delete;
        server_socket& operator= (const server_socket&) = delete;

        void init_socket();
        void bind_socket_to_ipv4_port( const char *ipv4_address="",
                int server_port=0);
        void bind_socket_to_ipv6_port( const char *ipv6_address="",
            int server_port=0);
        virtual const std::string& receive_data_from_client(bool save_client_addr)=0;
        virtual void send_short_mesg(const std::string &str)=0;
        void change_mesg_buffer_size(size_t size){max_buffer_size_=size;}
        std::string get_client_info(decltype(AF_INET) ip_version)const;
    private:
        void close_server_socket()const;
};
}