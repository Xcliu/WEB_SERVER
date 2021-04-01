#include "WEB_SERVER/utils/ThreadLib/write_read_lock.h"

using namespace SONNIE;

void Write_read_lock::reader_lock()
{
    std::unique_lock<std::mutex> lk(mtx_content);
    cond.wait(lk, [this] { return !this->check_writer(); }); //有这一句才可能实现写者优先
    if ((++reader_count) == 1)
    {
        mtx_rd.lock();
    }
    mtx_content.unlock();
}

void Write_read_lock::reader_unlock()
{
    std::lock_guard<std::mutex> lg(mtx_content);
    if ((--reader_count) <= 0)
    {
        mtx_rd.unlock();
    }
}

void Write_read_lock::writer_lock()
{
    if (priority_type == WRITER_PREFER)
    {
        //  如果指令exist_writer = true; mtx_rd.lock();可分割
        //  在一个writer释放一个writer尝试持锁时会出现, writer持锁
        //  但是exist_writer=false的状态, 所以最后再加一句exist_writer = true;
        exist_writer = true; //reader_lock()中条件变量配合实现写者优先
        mtx_rd.lock();
        exist_writer = true;
    }
    else if (priority_type == READER_PREFER)
    {
        mtx_rd.lock(); //存在连续的读者到来时，必须等到全部的读者读完才可能有机会
        exist_writer = true;
    }
}

void Write_read_lock::writer_unlock()
{
    {
        std::lock_guard<std::mutex> lg(mtx_content);
        exist_writer = false;
    }
    mtx_rd.unlock();
    cond.notify_all(); //唤醒全部被阻塞的读者
}