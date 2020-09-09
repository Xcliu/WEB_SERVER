#include <exception>

namespace SONNIE{

class socket_exception:std::exception{
    public:
        socket_exception(const char* _error_mesg);
        const char* what();



}; 



}