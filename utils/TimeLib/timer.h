#pragma onece
#include <array>
#include <chrono>
#include <ostream>

namespace SONNIE {

class Timer {
 public:
  using time_point_t =
      typename std::chrono::time_point<std::chrono::high_resolution_clock>;
  explicit Timer() = default;
  virtual ~Timer() = default;
  void save_time_point() {
    time_point_array_[latest_update_index_ % 2] =
        std::chrono::high_resolution_clock::now();
    latest_update_index_ = (latest_update_index_ + 1) % 2;
  }
  std::chrono::duration<double, std::milli> cal_time_interval_ms() const {
    return time_point_array_[(latest_update_index_ + 1) % 2] -
           time_point_array_[latest_update_index_ % 2];
  }

 public:
  int64_t latest_update_index_{0};
  std::array<time_point_t, 2> time_point_array_;
};

class Scope_timer : public Timer {
 public:
  explicit Scope_timer(std::ostream& out_s) : output_obj_(out_s) {
    save_time_point();
  };
  virtual ~Scope_timer() {
    save_time_point();
    output_obj_ << "cost: " << cal_time_interval_ms().count() << " ms."
                << std::endl;
  }

 public:
  std::ostream& output_obj_;
};

}  // namespace SONNIE