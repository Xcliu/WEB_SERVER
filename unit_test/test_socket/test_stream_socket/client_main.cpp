#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "WEB_SERVER/socket_lib/socket_common_define.h"

#define MAX 80
#define IP "127.0.0.1"
#define PORT 8888
#define SA struct sockaddr

void func(int sockfd)
{
    char buff[MAX];
    int n = 0;
    bzero(buff, sizeof(buff));
    printf("Enter the string : \n");
    while ((buff[n++] = getchar()) != '\n');
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("From Server : %s \n", buff);
}
  
int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }else{
        printf("Socket successfully created..\n");
    }
    bzero(&servaddr, sizeof(servaddr));
  
    // assign server IP, PORT
    in_addr dest; 
    memset(&dest,0,sizeof(dest));
    int temp=inet_pton(AF_INET,IP,&dest);
    if(temp < 0){
        printf("server address creation failed..\n");
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr = dest;
    servaddr.sin_port = htons(PORT);
  
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else{
        printf("connected to the server..\n");
    }

    // function for chat
    func(sockfd);
  
    // close the socket
    close(sockfd);
}