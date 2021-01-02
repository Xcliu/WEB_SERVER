/*
 * exception 可能会使得进程跳过join语句，使得线程资源不能被回收 
 *
 * 1.捕获任意异常后join线程，在转发异常
 * 2.利用结构体管理线程（与智能指针的做法类似，
 * 这里又有两种选择，一种是完全转交线程控制权，一种是利用引用控制线程。）
 */
#include <thread>
#include <iostream>
#include <unistd.h>
#include <exception>

class Guard_thread{
    private:
        std::thread &t;
    public:
        Guard_thread(std::thread & _t):t(_t){}
        virtual ~Guard_thread(){
            if(t.joinable()){
                t.join();
            }
        }
        Guard_thread(const Guard_thread &_gt)=delete; //不可copy和assignment
        Guard_thread operator=(const Guard_thread &_gt)=delete;
};

class Guard_thread_completely{
    private:
        std::thread t;
    public:
        Guard_thread_completely(std::thread &_t){
            t=std::move(_t);
        }
        virtual ~Guard_thread_completely(){
            if(t.joinable()){
                t.join();
            }else{
                throw std::logic_error("the empty thread is associated with memeber");
            }
        }
        Guard_thread_completely(const Guard_thread_completely &_gtc)=delete;
        Guard_thread_completely operator=(const Guard_thread_completely &_gtc)=delete;
};

int main(int argc, char *argv[]){
    std::string str="nothing important";
    //new thread task;
    std::thread t(
        [str]()->bool{
            sleep(3);
            std::cout<< str<<std::endl;
            return true;
        }
    );

    //Guard_thread g_thread(t);
    Guard_thread_completely g_thread(t);
    // main thread task
    try{
        throw std::runtime_error("fool exception");
    }catch(const std::exception &excp){
        std::cout<<excp.what()<<std::endl;
        //throw;
    }
    //t.join();
    return 0; //main thread  结束之前没有是的子线程解耦合会导致程序异常退出
}


// int main(int argc,char *argv[]){
//     bool tag=argc>=2?true:false;
//     std::thread t;
//     try{
//         t=std::thread(
//             [tag](){
//                 if(tag){
//                     sleep(2);
//                     std::cout<<"new thread is running"<<std::endl;
//                 }else{
//                     throw std::logic_error("new thread tag error!");
//                 }
//             }
//         );//thred 的入口函数抛出异常，那么thread的对象为empty
//     }catch(const std::exception &excp){
//         if(t.joinable()){
//             std::cout <<"joinable" <<std::endl;
//             t.join();
//         }
//         std::cout<<excp.what()<<std::endl;
//         throw excp;
//     }
//     t.join();
//     return 0;
// }