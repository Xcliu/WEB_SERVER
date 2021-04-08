#include <chrono>  // std::chrono::seconds
#include <iostream>
#include <mutex>
#include <random>
#include <sstream>
#include <string>
#include <thread>

#include "WEB_SERVER/utils/ThreadLib/thread_pool.hpp"

int generate_random_int(const int beg, const int end) {
  std::random_device sd;
  std::minstd_rand linear_ran(sd());
  std::uniform_int_distribution<int> dis(beg, end);
  return dis(linear_ran);
}

std::string get_tid() {
  std::ostringstream ss;
  ss << std::this_thread::get_id();
  return ss.str();
}

int task_func(std::string str) {
  std::string temp = get_tid() + " -> " + str;
  std::cout << temp << std::endl;
  std::this_thread::sleep_for(
      std::chrono::microseconds(generate_random_int(30, 40)));
  return 1;
}

int main(int argc, char *argv[]) {
  {
    SONNIE::Thread_pool<int, std::string> tp(3, 100, 0);
    std::cout << "thread pool size = 3 " << std::endl;
    for (int i = 0; i < 6; i++) {
      tp.add_task(task_func, std::to_string(i));
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "thread pool size = 1 " << std::endl;
    tp.decrease_pool_size(2);
    for (int i = 6; i < 10; i++) {
      tp.add_task(task_func, std::to_string(i));
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "thread pool size = 2 " << std::endl;
    tp.increase_pool_size(1);
    for (int i = 10; i < 20; i++) {
      tp.add_task(task_func, std::to_string(i));
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "thread pool size = 3 " << std::endl;
    tp.increase_pool_size(1);
    for (int i = 20; i < 26; i++) {
      tp.add_task(task_func, std::to_string(i));
    }
  }
  std::cout << "control block out of thread pool" << std::endl;
  return 0;
}