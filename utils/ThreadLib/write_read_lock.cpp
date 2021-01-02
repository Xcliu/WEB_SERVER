#include "write_read_lock.h"

using namespace SONNIE;

void Write_read_lock::reader_lock(){

    std::unique_lock<std::mutex> lk(mtx_content);
    cond.wait(lk,[this]{return !this->check_writer();});//有这一句才可能实现写者优先
    ++reader_count;
    if(reader_count==1){
        mtx_rd.lock();
    }
	mtx_content.unlock();
}

void Write_read_lock::reader_unlock(){
    mtx_content.lock();
	--reader_count;
    if(reader_count==0){
        mtx_rd.unlock();
    }
	mtx_content.unlock();
}

void Write_read_lock::writer_lock(){
    if(priority_type==WRITE_PREFER){
        exist_writer=true;//reader_lock()中条件变量配合实现写者优先
        mtx_rd.lock();
    }else if(priority_type==READE_PREFER){
        mtx_rd.lock();//存在连续的读者到来时，必须等到全部的读者读完在可能有机会
        exist_writer=true;
    }
}

void Write_read_lock::writer_unlock(){
	exist_writer=false;
    mtx_rd.unlock();
    cond.notify_all();//唤醒全部被阻塞的读者
}