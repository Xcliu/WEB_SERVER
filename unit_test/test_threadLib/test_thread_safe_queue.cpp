#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "WEB_SERVER/utils/ThreadLib/thread_safe_queue.hpp"

std::string get_tid() {
  std::ostringstream ss;
  ss << std::this_thread::get_id();
  return ss.str();
}

void consumer(SONNIE::Thread_safe_queue<std::string>& queue) {
  std::string str;
  queue.wait_and_pop(str);
  std::cout << "consumer: " << get_tid() << "->" << str << std::endl;
}

void producer(SONNIE::Thread_safe_queue<std::string>& queue) {
  queue.push(get_tid());
  std::cout << "producer: " << get_tid() << std::endl;
}

int main(int argc, char* argv[]) {
  SONNIE::Thread_safe_queue<std::string> queue(200);
  std::vector<std::thread> task_vec;

  for (int i = 0; i < 10; ++i) {
    if (i % 4 == 1) {
      task_vec.emplace_back(producer, std::ref(queue));
    } else {
      task_vec.emplace_back(consumer, std::ref(queue));
    }
  }

  for (auto& item : task_vec) {
    item.join();
  }
  return 0;
}
