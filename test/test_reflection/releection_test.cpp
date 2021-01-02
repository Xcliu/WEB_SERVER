#include "WEB_SERVER/utils/ReflectionLib/reflection.h"
#include <iostream>
#include <string>

// TODO(lxc,2021-1-2): 还没有测试

class Fool{
    public:
        static std::string a;
};
std::string Fool::a = "registed class success!";

REGISTER(Fool);

int main(int argc,char *argv[]){
    void* obj_ptr = SONNIE::GET_INSTANCE_BY_NAME("Fool");
    std::cout << static_cast<Fool*>(obj_ptr)->a << std::endl;
    return 0;
}