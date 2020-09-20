#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include<sys/sendfile.h>
/*for mac os*/
#if __APPLE__
    #include <sys/uio.h>
#elif
    #include <sys/sendfile.h>
#endif

#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include <iostream>

const int port = 8888;
int main(int argc,char *argv[])
{
    
    int sock;
    int connfd;

    struct sockaddr_in sever_address;
   
    bzero(&sever_address,sizeof(sever_address));

    sever_address.sin_family = AF_INET; //应改为 AF_INET
    sever_address.sin_addr.s_addr = htons(INADDR_ANY);// 应改为 htons 
    sever_address.sin_port = htons(8888);
 
    sock = socket(AF_INET,SOCK_STREAM,0);
 
    assert(sock>=0);
 
    int ret = bind(sock, (struct sockaddr*)&sever_address,sizeof(sever_address));
    assert(ret != -1);
 
    ret = listen(sock,1);
    assert(ret != -1);

    while(1)
    {
        struct sockaddr_in client;
        socklen_t client_addrlength = sizeof(client);
        connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength);

        if(connfd<0)
        {
           std::cout << "failed to accept the request from client"<<std::endl;;
        }
        else{
                char request[1024];
                recv(connfd,request,1024,0);
                request[strlen(request)+1]='\0';
                std::cout <<"received message from client:"<<std::endl; 
                std::cout<<request<<std::endl;
                char buf[520]="HTTP/1.1 200 ok\r\nconnection: close\r\n\r\n";//HTTP响应,HTTP报文的格式
                int s = send(connfd,buf,strlen(buf),0);//发送响应
                std::cout<<"the Http head has been sent to client: " << s << std::endl; 
                int fd_html=open("hello.html",O_RDONLY);
                off_t send_size=1024;
                struct sf_hdtr obj;
                sendfile(fd_html,connfd,0,&send_size,&obj,0);//零拷贝发送消息体
                close(fd_html);
                close(connfd);
        }  
    }
    return 0;
}