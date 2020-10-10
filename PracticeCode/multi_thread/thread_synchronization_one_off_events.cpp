#include <thread>
#include <future>
#include <iostream>
#include <cmath>
#include <unistd.h>
/*using promise*/
// static std::promise<double> pro;

// void producers_task(double tag){
//     sleep(3);
//     if(tag<0){
//         std::cout<<"error: must be positive"<<std::endl;
//     }else{
//         
//         pro.set_value(std::sqrt(tag));
//     }
// }

// void consumer_task(){
//     std::future<double> fut=pro.get_future();
//     std::cout<< fut.get()<<std::endl;
// }

// int main(int argc,char *argv[]){
//     std::cout<<"main thread ID::"<<std::this_thread::get_id()<<std::endl;  
//     std::thread thread_c(consumer_task);  
//     std::thread thread_p(producers_task,3.14);
//     thread_c.join();
//     thread_p.join();
//     return 0;
// }

/*
*using package_task
*/

// int main(int argc,char *argv[]){
//     auto pac=std::packaged_task<double(double)>([](double in)->double{
//         return in*in;
//     });
//     auto fut=pac.get_future();//由于pac 传递给thread_producers是被销毁，因此 fut 必须在生成pac 以后马上获得
//     //而不能放在子线程中获取。
//     std::thread thread_consumer(
//         [&fut](){
//             std::cout<< fut.get()<<std::endl;
//         }
//     );
//     std::thread thread_producers(std::move(pac),3.14);// convert to right value is necessary.
//     thread_consumer.join();
//     thread_producers.join();
//     return 0;
// }

/*
*using async 
*/

int main(int argc,char *argv[]){
    std::cout<<"main thread ID: "<<std::this_thread::get_id()<<std::endl;
    auto fut= std::async(std::launch::deferred,
        [](double in)->double{
            std::cout<<"task thread ID: "<<std::this_thread::get_id()<<std::endl;
            sleep(3);
            return in*in;
        }
    ,3.14);//async 的区别在于它可以自动创建新线程,即生产端由编译器负责
    fut.wait();
    std::thread thread_c([&fut](){
        std::cout<<fut.get()<<std::endl;
    });
    thread_c.join();
    return 0;
}