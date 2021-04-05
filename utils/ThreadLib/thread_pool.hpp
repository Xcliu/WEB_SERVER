#pragma once

#include "WEB_SERVER/utils/ThreadLib/thread_safe_queue.hpp"
#include <thread>
#include <functional>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

namespace SONNIE{
    
template< typename R, typename... Args>
class Thread_pool{
    public:
        using task_t = std::function<R(void)>;

        explicit Thread_pool(size_t sz, int64_t queue_tiemout)
                :thread_pool_size_(sz),task_queue_(queue_tiemout){}
        
        virtual ~Thread_pool(){
            exit_tag_ = true;
            for(size_t i = 0 ; i< thread_vec_.size(); ++i){
                thread_vec_[i].join();
            }
            std::cout << "destruct func" << std::endl;
        }
    
        void run(){
            for(size_t i = 0;i < thread_pool_size_; ++i){
                thread_vec_.emplace_back([this](){
                    while(true){
                        // 后续需要在这里给每个task设置一个超时时间
                        task_t task_obj; 
                        if(this->task_queue_.wait_and_pop(task_obj)){
                            task_obj();
                        // task_queue is empty and exit_tag_ is false;
                        }else if(exit_tag_){
                            break;
                        }
                    }
                });
            }
        }

        // NOTE(lxc): how to save the variable parameters for the next calling
        // 有两种解决方法 std::bind 生成一个新的void(void)的可调用对象
        // 或者使用std::tuple 来存参数包 
        void add_task(const std::function<R(Args...)> f, Args&&... args){
            if(!exit_tag_){
                task_queue_.push(std::bind(f,std::forward<Args...>(args...)));
            }
        }

    private:
        const size_t thread_pool_size_;
        std::vector<std::thread> thread_vec_;
        SONNIE::Thread_safe_queue<task_t> task_queue_;
        bool exit_tag_{false};
};

} // namspace SONNIE
