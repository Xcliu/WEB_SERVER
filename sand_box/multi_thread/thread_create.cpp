#include <unistd.h>

#include <iostream>
#include <string>
#include <thread>

using std::cout;
using std::endl;

void lambda_as_entry_func(std::thread &t) {
  const char str[] = "the new thread is running";
  t = std::thread(([str]() {
    sleep(3);
    printf("\n******* hardware concurrency %d********\n",
           std::thread::hardware_concurrency());
    cout << str << endl;
  })  // copy type input paraments
  );
}

void task(const std::string &s) { cout << ("task processed:" + s) << endl; }

void ordinary_entry_func(std::thread &t) {
  t = std::thread(task, "msg send to new thread");
}

class callable_class {
 private:
  const std::string name;
  std::uint64_t ID;

 public:
  explicit callable_class(std::string _name = "lxc", uint64_t _ID = 1701110156)
      : name(_name), ID(_ID) {}
  void operator()() {
    cout << "name: " << name << ";"
         << "ID " << ID << endl;
  }
};

void callable_obj_entry_func(std::thread &t) {
  auto obj = callable_class();
  t = std::thread(obj);
}

int main() {
  std::thread t;  // empty thread;
  callable_obj_entry_func(t);
  printf("\n****** welcome the new created thread!*****\n");
  t.join();  // 主线程被阻塞，知道t线程也执行完毕
  return 0;
}
