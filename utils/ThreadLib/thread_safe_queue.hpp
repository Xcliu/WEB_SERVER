#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
namespace SONNIE{

template<typename T>
class Thread_safe_queue{
    
    private:
        std::queue<T> raw_queue_;
        mutable std::mutex mtx_; 
        std::condition_variable con_var_;
        const int64_t time_out_;

    public:
        explicit Thread_safe_queue(const int64_t time_out):time_out_(time_out){}
        virtual ~Thread_safe_queue()=default;

        void push(T&& t){
            std::lock_guard<std::mutex> lg(mtx_);
            raw_queue_.push(std::forward<T>(t));
            con_var_.notify_one();
        }

        bool wait_and_pop(T& t){
            std::unique_lock<std::mutex> ul(mtx_);
            bool no_tiemout = true;

            if(time_out_ < 0){
                con_var_.wait(ul,[this](){ return !this->raw_queue_.empty();});
            }else{
                no_tiemout = con_var_.wait_for(ul,std::chrono::duration<int64_t, std::milli>(this->time_out_),
                                [this](){ return !this->raw_queue_.empty();});
            }

            if(no_tiemout){
                t = std::move(raw_queue_.front());
                raw_queue_.pop();
            }
            return no_tiemout;
        }

        bool try_pop(T& t){
            std::lock_guard<std::mutex> lg(mtx_);
            if(raw_queue_.empty()){
                return false;
            }else{
                t = std::move(raw_queue_.front());
                raw_queue_.pop();
                return true;
            }
        }

        bool empty(){
            std::lock_guard<std::mutex> lg(mtx_);
            return raw_queue_.empty();
        }
};

} // namespce SONNIE