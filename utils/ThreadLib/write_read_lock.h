#pragma once
#include <mutex>
#include <condition_variable>

#define WRITER_PREFER 0
#define READER_PREFER 1

namespace SONNIE
{

    class Write_read_lock
    {
    private:
        const bool priority_type;
        bool exist_writer;
        uint64_t reader_count;
        std::mutex mtx_rd;
        std::mutex mtx_content;
        std::condition_variable cond;

    public:
        explicit Write_read_lock(bool _priority = READER_PREFER) : priority_type(_priority), exist_writer(false), reader_count(0){};
        Write_read_lock(const Write_read_lock &wrl) = delete;
        Write_read_lock &operator=(const Write_read_lock &wrl) = delete;
        virtual ~Write_read_lock() {}
        void reader_lock();
        void reader_unlock();
        void writer_unlock();
        void writer_lock();
        bool check_writer() { return exist_writer; };
    };

    class Writer_lock_guard
    {
    private:
        Write_read_lock &wdl;

    public:
        Writer_lock_guard(Write_read_lock &_wdl) : wdl(_wdl)
        {
            wdl.writer_lock(); //lock during the construction proecss
        }
        virtual ~Writer_lock_guard()
        {
            wdl.writer_unlock();
        }
    };

    class Reader_lock_guard
    {
    private:
        Write_read_lock &wdl;

    public:
        Reader_lock_guard(Write_read_lock &_wdl) : wdl(_wdl)
        {
            wdl.reader_lock();
        }
        virtual ~Reader_lock_guard()
        {
            wdl.reader_unlock();
        }
    };

} //end of namespace