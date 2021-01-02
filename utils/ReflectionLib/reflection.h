#pragma once

#include <unordered_map>
#include <string>
#include <memory>

// NOTE(lxc,2021-1-2): 使用REGISTER(name)时会给name 对象分配堆内存，
// 这一部分内存需要使用REGISER机制的 client 来管理（delete） 

namespace SONNIE{

class Factory_Base{
    public:
        Factory_Base() = default;
        virtual ~ Factory_Base() = default;
        virtual void* Instance() = 0 ;
};

std::unordered_map<std::string, Factory_Base*>& name_to_factory();
void* GET_INSTANCE_BY_NAME(std::string name);

} // end of namespace SONNIE

#define REGISTER(name) \
    class Factory_##name : public SONNIE::Factory_Base{ \
     public:\
        void* Instance() override { \
            static void* class_instance = new name(); \
            return class_instance; \
        } \
    }; \
    void register_factory_##name(){ \
        if(SONNIE::name_to_factory().count(#name) == 0){\
            SONNIE::Factory_Base* f_obj = new Factory_##name();\
            SONNIE::name_to_factory().insert(std::make_pair(#name, f_obj));\
        } \
    } \
    __attribute__((constructor))void register_factory_##name();\
    __attribute__((destructor)) void release_factory_##name() {\
        delete SONNIE::name_to_factory().at(#name);\
    }
