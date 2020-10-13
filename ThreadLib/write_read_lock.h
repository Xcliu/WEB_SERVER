/*
* This lib is under testing 
* 2020-11-13
* lxc
*/

#pragma once
#include <mutex>
#include <condition_variable>

namespace SONNIE{

class Write_read_lock{
    private:
        static const bool WRITE_PREFER=0;
        static const bool READE_PREFER=1;
        const bool priority_type;
        bool exist_writer;
        unsigned long long reader_count;
        std::mutex mtx_rd;
        std::mutex mtx_content;
        std::condition_variable cond;
    public:
        explicit Write_read_lock(bool _priority=READE_PREFER):priority_type(_priority),exist_writer(false),reader_count(0){};
        Write_read_lock(const Write_read_lock &wrl)=delete;
        Write_read_lock &operator=(const Write_read_lock &wrl)=delete;
        virtual ~Write_read_lock(){}
        void reader_lock();
        void reader_unlock();
        void writer_unlock();
        void writer_lock();
};

class Writer_lock_guard{
    private:
        Write_read_lock & wdl;
    public:
        Writer_lock_guard(Write_read_lock & _wdl):wdl(_wdl){
            wdl.writer_lock();
        }
        virtual ~Writer_lock_guard(){
            wdl.writer_unlock();
        }
};

class Reader_lock_guard{
    private:
        Write_read_lock & wdl;
    public:
        Reader_lock_guard(Write_read_lock & _wdl):wdl(_wdl){
            wdl.reader_lock();
        }
        virtual ~Reader_lock_guard(){
            wdl.reader_unlock();
        }
};

}//end of namespace