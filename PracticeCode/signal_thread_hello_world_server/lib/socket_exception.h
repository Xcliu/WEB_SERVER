#ifndef SOCKET_EXCEPTION_H_
#define SOCKET_EXCEPTION_H_
#include <exception>
#include <string>
#include <iostream>

namespace SONNIE{

#define code_position_info() printf("\n******\nfunction: %s() in file: %s (line: %d)\n******\n", __func__ , __FILE__, __LINE__ );

class socket_exception:std::exception{
    public:
        socket_exception(const char* _error_mesg);
        const char* what();
    private:
        std::string error_mesg; 
}; 
}
#endif