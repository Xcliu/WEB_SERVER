#ifndef SOCKET_EXCEPTION_H_
#define SOCKET_EXCEPTION_H_

#include <exception>
#include <string>
namespace SONNIE{

class socket_exception:std::exception{
    public:
        socket_exception(const char* _error_mesg);
        const char* what();
    private:
        std::string error_msg; 
}; 
}

#endif