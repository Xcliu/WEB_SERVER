#include <thread>
#include <future>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <exception>
/*using promise*/

// static std::promise<double> pro;

// void producers_task(double tag){
//     sleep(2);
//     if(tag<0){
//         pro.set_exception(
//             std::make_exception_ptr(std::logic_error("Error: input for sqrt should be positive."))
//         );
//     }else{
//         pro.set_value(std::sqrt(tag));
//     }
// }

// void consumer_task(){
//     std::future<double> fut=pro.get_future();
//     double temp;
//     try{
//         temp=fut.get();
//     }catch(std::logic_error & le){
//         std::cout<<le.what()<<std::endl;
//         return;
//     }
//     std::cout<< temp <<std::endl;
// }

// int main(int argc,char *argv[]){
//     std::cout<<"main thread ID::"<<std::this_thread::get_id()<<std::endl;  
//     std::thread thread_c(consumer_task);  
//     std::thread thread_p(producers_task,-3.14);
//     thread_c.join();
//     thread_p.join();
//     return 0;
// }

/*
*using package_task
*/

// int main(int argc,char *argv[]){
//     auto pac=std::packaged_task<double(double)>([](double in)->double{
//         sleep(2);
//         if(in<0){
//             throw std::out_of_range("Error: nothing important");
//         }
//         return in*in;
//     });
//     auto fut=pac.get_future();//由于pac 传递给thread_producers是被销毁，因此 fut 必须在生成pac 以后马上获得
//     //而不能放在子线程中获取。
//     std::thread thread_consumer(
//         [&fut](){
//             double temp;
//             try{
//                 temp=fut.get();
//             }catch(std::out_of_range &ep){
//                 std::cout<< ep.what()<<std::endl;
//                 return;
//             }
//             std::cout<< temp <<std::endl;
//         }
//     );
//     std::thread thread_producers(std::move(pac),-3.14);// convert to right value is necessary.
//     thread_consumer.join();
//     thread_producers.join();
//     return 0;
// }

/*
*using shared_future by async 
*/

int main(int argc,char *argv[]){
    std::cout<<"main thread ID: "<<std::this_thread::get_id()<<std::endl;
    auto fut= std::async(std::launch::async,
        [](double in)->double{
            std::cout<<"task thread ID: "<<std::this_thread::get_id()<<std::endl;
            sleep(3);
            return in*in;
        }
    ,3.14);//async 的区别在于它可以自动创建新线程,即生产端由编译器负责
    fut.wait();
    
    std::mutex mtx;
    auto fut_c1=fut.share();
    std::thread thread_c1([&fut_c1,&mtx](){
        std::unique_lock<std::mutex> ul(mtx); // the mutex used to protect outstream(a kind of shared resources)
        std::cout<<"thread ID: "<< std::this_thread::get_id()<<" ; "<<fut_c1.get()<<std::endl;
    });
    
    // auto fut_c2=fut.share(); //error in auto fut_c1=fut.share(); the std::move() is default,
    // so the fut instance is destoryed after auto fut_c1=fut.share();
    auto fut_c2=fut_c1;//
    std::thread thread_c2([&fut_c2,&mtx](){
        std::unique_lock<std::mutex> ul(mtx);
        std::cout<<"thread ID: " << std::this_thread::get_id()<<" ; "<<fut_c2.get()<<std::endl;
    });

    thread_c2.join();
    thread_c1.join();
    return 0;
}


