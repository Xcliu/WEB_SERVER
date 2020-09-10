#include "socket_exception.h"

using namespace SONNIE;

socket_exception::socket_exception(const char * _error_msg){
    error_msg=_error_msg;
}

const char *socket_exception::what(){
    return error_msg.c_str();
}
