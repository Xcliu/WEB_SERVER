#include "WEB_SERVER/utils/ThreadLib/thread_pool.hpp"

#include <string>
#include <iostream>
#include <thread>
#include <sstream>
#include <mutex>

static std::mutex mtx;

std::string get_tid(){
    std::ostringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

void task_func(std::string str){
    std::lock_guard<std::mutex> lg(mtx);
    std::cout << get_tid() << " -> " << str << std::endl; 
}

int main(int argc,char *argv[]){
    {
        SONNIE::Thread_pool<void, std::string> tp(3,100);
        tp.run();
        for(int i = 0; i<20 ; i++){
            tp.add_task(task_func,std::to_string(i));
        }
    }
    std::cout << "control block out of thread pool" << std::endl;
    return 0;
}