#include "server_socket.h"
#include "socket_exception.h"
using namespace SONNIE;


server_socket::server_socket(
        decltype(AF_INET) _ip_version,
        decltype(SOCK_STREAM) _socket_type
    ):ip_version(_ip_version),socket_type(_socket_type)
{
    server_socket_fd=-1;
    server_socket_info_ipv4=nullptr;
    client_socket_info_ipv4=nullptr;

    server_socket_info_ipv6=nullptr;
    client_socket_info_ipv6=nullptr;
    //std::cout << "fuck lib"<< std::endl;

    max_buffer_size=1024;// default size 1KB;
    recvd_mesg_buffer="";
    send_mesg_buffer="";
}

server_socket::~server_socket(){
    delete server_socket_info_ipv4;
    delete server_socket_info_ipv6;
    delete client_socket_info_ipv4;
    delete client_socket_info_ipv6;
}

void server_socket::create_socket(){
    if(socket_type==SOCK_DGRAM){
        /*not necessary*/
        server_socket_fd=socket(ip_version,socket_type,IPPROTO_UDP);
    }else if(socket_type==SOCK_STREAM){
        server_socket_fd=socket(ip_version,socket_type,0);
    }
    // std::cout<< server_socket_fd <<std::endl;
    if(server_socket_fd<0){
        throw socket_exception("failed to create a socket");
    }
    return ;
}

void server_socket::bind_socket_to_ipv4_port(
            const char *_ipv4_address,
            int _server_port
        )
{
    if(ip_version!=AF_INET){
        throw socket_exception("non_ipv4_socket can not be binded to ipv4 address!");
    }

    delete server_socket_info_ipv4;
    server_socket_info_ipv4=new socket_info_addr_ipv4();
    if(strcmp(_ipv4_address,"")!=0&&_server_port!=0){
        server_socket_info_ipv4->set_ip_port_ipv4(_ipv4_address,_server_port);
    }

    const sockaddr_in *temp=server_socket_info_ipv4->get_sockaddr();
    socklen_t socket_size=sizeof(*temp);
    int result=bind(
        server_socket_fd, 
        (struct sockaddr*)temp,
        socket_size 
    );
    if(result<0){
        systemcall_error_info();
        throw socket_exception("failed to bind socket!");
    }
    return ;
}

void server_socket::bind_socket_to_ipv6_port(
            const char * _ipv6_address,
            int _server_port
        )
{
    if(ip_version!=AF_INET6){
        throw socket_exception("non_ipv6_socket can not be binded to ipv6 address!");
    }

    delete  server_socket_info_ipv6;
    server_socket_info_ipv6=new socket_info_addr_ipv6();
    if(strcmp(_ipv6_address,"")!=0&&_server_port!=0){
        //std::cout << "check failed  "<<std::endl;
        server_socket_info_ipv6->set_ip_port_ipv6(_ipv6_address,_server_port);
    }
    const sockaddr_in6 *temp=server_socket_info_ipv6->get_sockaddr();
    socklen_t socket_size=sizeof(*temp);
    int result=bind(
        server_socket_fd, 
        (struct sockaddr*)temp,
        socket_size
    );
    if(result<0){
        systemcall_error_info();
        throw socket_exception("failed to bind socket!");
    }
    return ;
}

void server_socket::close_server_socket()const{
    if(close(server_socket_fd)==-1){
        throw socket_exception("failed to close connected scoket");
    }
}

void server_socket::set_mesg_buffer_size(size_t size){
    max_buffer_size=size;
}


std::string server_socket::get_client_info(decltype(AF_INET) _ip_version)const{
    if(_ip_version==AF_INET){
        if(client_socket_info_ipv4==nullptr){
            return "";
        }
        sockaddr_in *temp=client_socket_info_ipv4->get_sockaddr();

        //checking codes which should be removed before using  
        // temp->sin_addr.s_addr=INADDR_ANY;
        // temp->sin_port=htons(4242);
        
        char dst[max_buffer_size];
        if(inet_ntop(AF_INET,&(temp->sin_addr),dst,max_buffer_size)==nullptr){
            systemcall_error_info();
            throw socket_exception("fail to tranfrom ip to string");
        }
        return std::string(dst)+";"+std::to_string(ntohs(temp->sin_port));
    }else if(_ip_version==AF_INET6){
        if(client_socket_info_ipv6==nullptr){
            return "";
        }
        sockaddr_in6 *temp=client_socket_info_ipv6->get_sockaddr();
        
        //checking codes which should be removed before using  
        // (temp->sin6_addr)=in6addr_any;
        // temp->sin6_port=htons(4243);

        char dst[max_buffer_size];
        if(inet_ntop(AF_INET6,&(temp->sin6_addr),dst,max_buffer_size)==nullptr){
            systemcall_error_info();
            throw socket_exception("fail to tranfrom ip to string");
        }
        return std::string(dst)+";"+std::to_string(ntohs(temp->sin6_port));
    }
    return "";
    
}

