#include <chrono>
#include <iostream>
#include <thread>

#include "WEB_SERVER/utils/TimeLib/timer.h"

void f() { std::this_thread::sleep_for(std::chrono::seconds(1)); }

int main(int argc, char *argv[]) {
  {  // auto t1 = std::chrono::high_resolution_clock::now();
    SONNIE::Scope_timer timer(std::cout);
    f();
    // auto t2 = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
    // std::cout << "cost: " << fp_ms.count() << " ms " << std::endl;
  }
  return 0;
}