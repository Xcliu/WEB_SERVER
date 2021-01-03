#pragma once
#include <exception>
#include <string>
#include <iostream>
#include "utils/debug_tool/debug_common_info.h"

namespace SONNIE{

class socket_exception:std::exception{
    public:
        socket_exception(const char* _error_mesg);
        const char* what();
    private:
        std::string error_mesg; 
};
 
} // end of namespace SONNIE