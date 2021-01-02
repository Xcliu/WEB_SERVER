#include <unordered_map>
#include <string>
#include <iostream>
#include "reflection.h"

namespace SONNIE{

std::unordered_map<std::string, Factory_Base*>& name_to_factory(){
    static auto result = new std::unordered_map<std::string,Factory_Base*>();
    return *result;
}

void* GET_INSTANCE_BY_NAME(std::string name){
    if(name_to_factory().count(name) == 1){
        return name_to_factory().at(name)->Instance();
    }else{
        return nullptr;
    }
}

} // end of SONNIE namespace

// temp testing code 
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

