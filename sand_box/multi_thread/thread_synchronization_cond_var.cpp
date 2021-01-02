#include <thread>
#include <iostream>
#include <unistd.h>
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <ctime>

static std::queue<std::string> data_queue;
static std::mutex mtx;
static std::condition_variable cond;

void producers_task(int tag,const std::string &str){
    if(tag){
        std::cout<<"child thread ID: "<<std::this_thread::get_id()<<std::endl;
        std::lock_guard<std::mutex> g(mtx);
        char s[16];
        for (int i=0;i<tag;++i){
            std::time_t now=std::time(0);
            sprintf(s, "%09lX",now);
            data_queue.push(str+s);
            sleep(1);
        }
        cond.notify_all();
    }else{
        std::cout<<"child thread do noting"<<std::endl;
    }
}

void consumer_task(){
    std::unique_lock<std::mutex> lk(mtx);
    cond.wait(lk,[](){return !data_queue.empty();});
    std::string &item=data_queue.front();
    std::cout<<item<<std::endl;
    data_queue.pop();
    lk.unlock();
}

int main(int argc,char *argv[]){
    std::thread thread_c1(consumer_task); 
    std::thread thread_c2(consumer_task);
    std::thread thread_p(producers_task,2,"lxc"); 
    sleep(1);
    std::cout<<"hardware_concurrency: "<<std::thread::hardware_concurrency()<<std::endl;
    std::cout<<"main thread ID: "<<std::this_thread::get_id()<<std::endl;
    thread_p.join();
    thread_c1.join();
    thread_c2.join();
    return 0;
}