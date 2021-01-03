#include "socket_exception.h"

using namespace SONNIE;

socket_exception::socket_exception(const char * _error_mesg){
    error_mesg=_error_mesg;
}

const char *socket_exception::what(){
    return error_mesg.c_str();
}
