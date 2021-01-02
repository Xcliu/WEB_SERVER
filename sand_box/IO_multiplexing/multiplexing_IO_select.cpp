#include <exception>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
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
int fd_array[fd_size]{-1,-1,-1};

void creat_fd_array(){
    /*must be set as nonblock*/
    int return_fd=open("./read.txt",O_RDONLY|O_CREAT|O_NONBLOCK);
    if(return_fd<0){
        systemcall_error_info();
        exit(1);
    }else{
        fd_array[0]=return_fd;
    }

    return_fd=open("./read_exist.txt",O_RDONLY|O_NONBLOCK);
    if(return_fd<0){
        systemcall_error_info();
        exit(1);
    }else{
        fd_array[1]=return_fd;
    }

    return_fd=open("./write.txt",O_RDWR|O_CREAT|O_NONBLOCK);
    if(return_fd<0){
        systemcall_error_info();
        exit(1);
    }else{
        fd_array[2]=return_fd;
    }
}

int main(int argc,char *argv[]){
    
    fd_set *R_fd_ptr=new fd_set; 
    fd_set *W_fd_ptr=new fd_set;
    FD_ZERO(R_fd_ptr);FD_ZERO(W_fd_ptr);

    creat_fd_array();

    for(int i=0;i<3;++i){
        FD_SET(fd_array[i],R_fd_ptr);
    }
    FD_SET(fd_array[2],W_fd_ptr);

    timeval set_time;set_time.tv_sec=2;

    /*
    nfds shouled be set as amx(r_fd,w_fd,e_fd)+1
    */
    int total_num=select(6,R_fd_ptr,W_fd_ptr,nullptr,&set_time);
    if(total_num<0){
        systemcall_error_info();
        exit(1);
    }else if(total_num==0){
        printf("time out\n");
        exit(EXIT_FAILURE);
    }

    if(FD_ISSET(fd_array[0],R_fd_ptr)){
        printf("./read.txt is ready\n");
    }

    if(FD_ISSET(fd_array[1],R_fd_ptr)){
        printf("./read_exist.txt is ready\n");
    }

    if(FD_ISSET(fd_array[2],W_fd_ptr)){
        printf("./write.txt is ready\n");
    }
    delete R_fd_ptr; delete W_fd_ptr;
    for(int i=0;i<fd_size;++i){
        close(fd_array[i]);
    }
    
    return 0;
}




