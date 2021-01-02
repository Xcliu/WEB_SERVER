/*
*used for open()
*/
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
/*
* used for system call poll()
*/
#include <poll.h>

/*
used for macro system_error_info
*/
#include <stdio.h>
#include <sys/errno.h>
#include <string.h>
#define systemcall_error_info() fprintf(stderr, "*** ERROR %s, failed:%d(%s)\n", __func__,errno, strerror(errno));

/*
used for exit 
*/
#include <stdlib.h>

//open some files and return fd
static const unsigned int fd_size=3;
pollfd fd_array[fd_size];

void creat_fd_array(){
    /*must be set as nonblock*/
    int return_fd=open("./read.txt",O_RDONLY|O_CREAT|O_NONBLOCK);
    if(return_fd<0){
        systemcall_error_info();
        exit(1);
    }else{
        fd_array[0]={return_fd,POLLIN,POLL_IN};
    }

    return_fd=open("./read_exist.txt",O_RDONLY|O_NONBLOCK);
    if(return_fd<0){
        systemcall_error_info();
        exit(1);
    }else{
        fd_array[1]={return_fd,POLL_IN,POLL_IN};
    }

    return_fd=open("./write.txt",O_RDWR|O_CREAT|O_NONBLOCK);
    if(return_fd<0){
        systemcall_error_info();
        exit(1);
    }else{
        fd_array[2]={return_fd,POLL_OUT|POLL_IN,POLL_OUT|POLL_IN};
    }
}

int main(int argc,char *argv[]){
    
    creat_fd_array();

    timeval set_time;set_time.tv_sec=2;

    /*
    nfds shouled be set as sum(r_fd,w_fd,e_fd)+1?
    */
    int total_num=poll(fd_array,fd_size,1000);
    if(total_num<0){
        systemcall_error_info();
        exit(1);
    }else if(total_num==0){
        printf("time out\n");
        exit(EXIT_FAILURE);
    }

    if(fd_array[0].revents&POLL_IN){
        printf("./read.txt is ready\n");
    }

    if(fd_array[1].revents&POLL_IN){
        printf("./read_exist.txt is ready\n");
    }

    if((fd_array[2].revents&POLL_IN) && (fd_array[2].revents&POLL_OUT) ){
        printf("./write.txt is ready\n");
    }
    return 0;
    for(int i=0;i<fd_size;++i){
        close(fd_array[i].fd);
    }
}
