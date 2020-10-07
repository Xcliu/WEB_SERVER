/*
*used for open()
*/
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
/*
*error info
*/
#include "./system_call_erro_info.h"
/*
used for exit 
*/
    #include <stdlib.h>

int main(int argc,char *argv[]){
    int fifo_fd=open("./fifo_1",O_RDWR);
    if(fifo_fd<0){
        systemcall_error_info();
        exit(EXIT_FAILURE);
    }
    char str[]{"fifo: hello!"};
    size_t temp=write(fifo_fd,str,sizeof(str));
    if(temp<0){
        systemcall_error_info();
        exit(EXIT_FAILURE);
    }
    return 0;
}