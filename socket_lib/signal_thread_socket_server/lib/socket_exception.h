#ifndef SOCKET_EXCEPTION_H_
#define SOCKET_EXCEPTION_H_
#include <exception>
#include <string.h> //used for strerror()
#include <string>
#include <iostream>

#define code_position_info() \
    printf("\n******\nfunction: %s() in file: %s (line: %d)\n******\n", \
             __func__ , __FILE__, __LINE__ );

#define systemcall_error_info() \
    fprintf(stderr, "*** ERROR %s, failed:%d(%s)\n", \
            __func__,errno, strerror(errno));

#define error_exit() exit(EXIT_FAILURE)

namespace SONNIE{

class socket_exception:std::exception{
    public:
        socket_exception(const char* _error_mesg);
        const char* what();
    private:
        std::string error_mesg; 
};
 
} // end of namespace SONNIE
#endif