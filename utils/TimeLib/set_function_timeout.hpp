#pragma once
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <utility>

namespace SONNIE {

enum func_status { NORMAL = 0, TIME_OUT = 1 };

template <typename R, typename... Args>
class func_with_time_out {
 public:
  union return_data {
    R value;
    std::remove_reference<R>* place_holder;
  };

 public:
  explicit func_with_time_out() = default;
  virtual ~func_with_time_out() = default;
  std::pair<func_status, return_data> run_func(
      int64_t tiemout_ms, const std::function<R(Args...)> f, Args&&... args) {
    auto task_future =
        std::async(std::launch::async, f, std::forward<Args...>(args...));
    auto status = task_future.wait_for(std::chrono::milliseconds(tiemout_ms));
    if (status == std::future_status::timeout) {
      union return_data return_val;
      return_val.place_holder = nullptr;
      return {TIME_OUT, return_val};
    } else if (status == std::future_status::ready) {
      return {NORMAL, {task_future.get()}};
    }
  }
};

}  // namespace SONNIE
