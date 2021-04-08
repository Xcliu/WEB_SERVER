/**
 * 1.加锁主要是为了实现线程对共享数据的互斥访问，但是无法指定线程对数据的的访问顺序（由同步操作实现）
 *
 * 2.互斥锁类型 std::mutex ,加锁和锁的释放操作lock， unlock
 *
 * 3.std::unique_lock 和 std::lock_guard 可以托管muetx，实现锁的自动释放
 *
 * 4.还有特殊的读者锁 std::shared_lock,不在c++标准库中，在boost library中
 * #include <boost/thread/shared_mutex.hpp>
 *
 *
 */
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "write_read_lock.hpp"

class str_vector {
 private:
  std::vector<std::string> *data;
  // std::mutex mtx; //no need to be initialized in constructor.
  std::mutex mtx;
  SONNIE::shared_lock_guard<std::mutex> slg;

 public:
  str_vector() : slg(std::move(mtx)) {
    data = new std::vector<std::string>;
    // memset(data,0,sizeof(*data));
  }
  virtual ~str_vector() { delete data; }
  std::string end_element() {
    sleep(3);
    std::string temp;
    // mtx.lock();
    slg.reader_lock();
    if (data->empty()) {
      // mtx.unlock();
      throw std::runtime_error("empty contianer!");
    }
    temp = *--(data->end());  // (*data)[data->size()-1]
    // mtx.unlock();
    printf("end_element running!\n");
    return temp;
  }
  void add_back(std::string &&str) {
    // sleep(3);
    // mtx.lock();
    slg.writer_lock();
    data->push_back(std::forward<std::string>(str));
    // mtx.unlock();
    printf("add running!\n");
  }
  friend std::ostream &operator<<(std::ostream &out, str_vector &obj) {
    obj.slg.reader_lock();
    for (auto item : *(obj.data)) {
      out << item << ";";
    }
    out << std::endl;
    return out;
  }
};

int main(int argc, char *argv[]) {
  std::string temp = "lxc";
  str_vector sv_obj;
  std::thread t(&str_vector::add_back, std::ref(sv_obj), std::move(temp));
  try {
    std::cout << sv_obj.end_element() << std::endl;
  } catch (std::runtime_error &re) {
    std::cout << re.what() << std::endl;
  }
  t.join();
  return 0;
}
