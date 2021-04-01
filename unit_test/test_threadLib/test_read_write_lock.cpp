#include "WEB_SERVER/utils/ThreadLib/write_read_lock.h"
#include <memory>
#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace SONNIE;

std::string get_tid()
{
    std::ostringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

void w_func(Write_read_lock &wrl, std::string &obj)
{
    Writer_lock_guard wlg(wrl);
    obj = get_tid();
    std::cout << "writer: " << get_tid() << " -> " << obj << std::endl;
}

void r_func(Write_read_lock &wrl, const std::string &obj)
{
    Reader_lock_guard rlg(wrl);
    std::cout << "reader: " << get_tid() << " -> " << obj << std::endl;
}

void multi_reader_writer()
{

    Write_read_lock wrl(READER_PREFER);
    std::string obj{"init_str"};
    std::vector<std::thread> all_t; //the default constructor of thread should not be called
    for (int i = 0; i < 10; ++i)
    {
        if (i % 4 == 1){
            all_t.push_back(std::thread(w_func, std::ref(wrl), std::ref(obj)));
        }else{
            all_t.push_back(std::thread(r_func, std::ref(wrl), std::ref(obj)));
        }
    }

    for (auto &item : all_t)
    {
        item.join();
    }
}

/**
*经验：如果thred报 “attempt to use a deleted function“这样的error info,往往
*是作为线程入口的可调用对象的形参不匹配。
*
*/

int main(int argc, char *argv[])
{
    multi_reader_writer();
    return 0;
}
