#include "write_read_lock.h"
#include <memory>
#include <thread>
#include <iostream>
#include <vector>

using namespace SONNIE;

void w_func(Write_read_lock &wrl,std::shared_ptr<char> up,
    size_t sz,char c)
{
    Writer_lock_guard wlg(wrl);
    for(size_t i=0;i<sz;++i){
        *(up.get()+i)=c+i;
    }
}

void r_func(Write_read_lock &wrl,
    const std::shared_ptr<char> up,size_t sz,std::vector<std::string> &result)
{
    Reader_lock_guard rlg(wrl);
    
    result.push_back("tid: ");
    auto temp=std::this_thread::get_id();
    //result.push_back((char *)&temp);
    // 注意这里把十六进制数转string的方法
    result.push_back("-->");

    //std::cout<<"tid:"<<std::this_thread::get_id()<<"-->";
    for(size_t i=0;i<sz;++i){
        auto temp=(up.get()+i);
        result.push_back(temp);
        //std::cout<<*(up.get()+i)<<",";
    }
    //std::cout<<std::endl;
}

/**
*经验：如果three报 “attempt to use a deleted function“这样的error info,往往
*是作为线程入口的可调用对象的形参不匹配。
*
*/

int main(int argc,char *argv[]){
    size_t sz=10;
    std::shared_ptr<char> up(new char[sz],[](char *p){delete [] p;});
    //需要思考一个问题：智能指针的引用计数可能不是线程安全的，这样会导致bug存在！
    for(size_t i=0;i<sz;++i){
        *(up.get()+i)='a';
    }// memory allocate and initialization
    size_t reader_sz=4; 
    Write_read_lock wrl(READE_PREFER);
    std::vector<std::thread> reader_t;//the default constructor of thread should not be called 
    for(int i=0;i<reader_sz;++i){
        std::vector<std::string> result;
        reader_t.push_back(std::thread(r_func,std::ref(wrl),up,sz,std::ref(result)));
    }

    std::thread writer_t1(w_func,std::ref(wrl),up,sz,'a');
    
    for(auto &item:reader_t){
        item.join();
    }
    writer_t1.join();
    return 0;
}
