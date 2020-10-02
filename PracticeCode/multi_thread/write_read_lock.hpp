#pragma once

#include <mutex>

namespace SONNIE{

template<typename T>
class shared_lock_guard{
    private:
        T mtx;
        bool exist_writer;
        std::mutex p_mtx;
    public:
        shared_lock_guard(T &&_lock):exist_writer(false),mtx(_lock){}
        shared_lock_guard(const T &_lock)=delete;
        shared_lock_guard &operator=(const T & shared_lock_guard)=delete;
        virtual ~shared_lock_guard(){
            mtx.unlock();
        }
        void reader_lock(){
            p_mtx.lock();
            if(exist_writer){
                p_mtx.unlock();
                mtx.lock();
                return;
            }
            p_mtx.unlock();
        }
        void writer_lock(){
            mtx.lock();
            p_mtx.lock();
            exist_writer=true;
            p_mtx.unlock();
        }
};

}//end of namespace