#include "WEB_SERVER/utils/TimeLib/set_function_timeout.hpp"
#include "WEB_SERVER/utils/TimeLib/timer.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

int task(std::string str){
    // std::cout << str << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return 1;
}

int main(int argc,char *argv[]){
    SONNIE::func_with_time_out<int,std::string> f_tiemout_obj;
    std::pair<SONNIE::func_status,SONNIE::func_with_time_out<int,std::string>::return_data> result;
    {
        SONNIE::Scope_timer timer(std::cout);
        result = f_tiemout_obj.run_func(101,task,"haha");
    }
    if(result.first == SONNIE::NORMAL){
        std::cout << "normal " << result.second.value << std::endl;
    }else{
        std::cout << "time out "<< std::endl; 
    }
    return 0;
}
