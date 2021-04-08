#pragma once

#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "WEB_SERVER/utils/ThreadLib/thread_safe_queue.hpp"

namespace SONNIE {

template <typename R, typename... Args>
class Thread_pool {
 public:
  using task_t = std::function<R(void)>;
  // the return type of task cant not be "void"
  using task_r_t = typename task_t::result_type;

  explicit Thread_pool(size_t sz, int64_t queue_tiemout,
                       int64_t task_timeout_ms)
      : task_queue_(queue_tiemout), task_timeout_ms_(task_timeout_ms) {
    increase_pool_size(sz);
  }

  virtual ~Thread_pool() {
    exit_tag_ = true;
    for (size_t i = 0; i < pool_tail_ptr_; ++i) {
      thread_vec_[i].join();
    }
  }

  void increase_pool_size(int delta_num) {
    for (int i = 0; i < delta_num; ++i) {
      int index = pool_tail_ptr_;
      if (index <= static_cast<int>(thread_vec_.size()) - 1) {
        thread_vec_[index] = std::thread(task_lambad_, index);
        task_status_[index].clear();
        ++pool_tail_ptr_;
      } else {
        thread_vec_.emplace_back(task_lambad_, index);
        task_status_.emplace_back();
        ++pool_tail_ptr_;
      }
    }
  }

  void decrease_pool_size(int delta_num) {
    delta_num = delta_num >= pool_tail_ptr_ ? pool_tail_ptr_ / 2 : delta_num;
    int count = 0;
    while (count < delta_num) {
      int index = pool_tail_ptr_ - 1;
      task_status_[index].can_be_deleted_ = true;
      thread_vec_[index].join();
      --pool_tail_ptr_;
      ++count;
    }
  }

  void add_task(const std::function<R(Args...)> f, Args&&... args) {
    if (!exit_tag_) {
      task_queue_.push(std::bind(f, std::forward<Args...>(args...)));
    }
  }

 private:
  std::function<void(int)> task_lambad_{[this](int index) {
    while (true) {
      // TODO(lxc): 后续需要给每个task设置一个超时时间
      // 1.
      // 开一个新线程，在新线程中执行task，通过线程超时控制来实现，但是这这样做线程池就没意义了
      // 2. ？
      task_t task_obj;
      if (this->task_queue_.wait_and_pop(task_obj)) {
        task_status_[index].latest_return_val_ = task_obj();
      } else if (exit_tag_) {
        break;
      }
      // NOTE(lxc) exit_tag_ 和 can_be_deleted_ 的区别在于
      // 完全销毁线程池之前(exit_tag_)需要把task_queue中的task执行完
      // 但是线程池缩容时(can_be_deleted_)则不需要等待清空task_queue
      if (task_status_[index].can_be_deleted_) {
        break;
      }
    }
  }};
  struct task_status_info {
    task_r_t latest_return_val_;
    // NOTE(lxc):up to now, the return vaule of task has not be used.
    bool can_be_deleted_{false};

    task_status_info() = default;

    void clear() { can_be_deleted_ = false; }
  };

  int pool_tail_ptr_{0};

  // the item having same index in thread_vec_ and task_status_
  // realize a mapping from thread instance to task_status_info;
  std::vector<std::thread> thread_vec_;
  std::vector<task_status_info> task_status_;

  SONNIE::Thread_safe_queue<task_t> task_queue_;
  bool exit_tag_{false};
  const int64_t task_timeout_ms_;
};

}  // namespace SONNIE
