#ifndef SOCKET_RELATED_HEAD_FILE_H_
#define SOCKET_RELATED_HEAD_FILE_H_

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

#endif