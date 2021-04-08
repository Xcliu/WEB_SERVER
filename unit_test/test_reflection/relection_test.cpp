#include <iostream>
#include <string>

#include "reflection.h"

// utils 中 reflection lib 的通常用法

class BaseExecutor {
 public:
  BaseExecutor() = default;
  virtual ~BaseExecutor() = default;
  virtual void do_exec(int a) = 0;
};

class PlusExecutor : public BaseExecutor {
  void do_exec(int a) override { std::cout << ++a << std::endl; }
};

class MinusExecutor : public BaseExecutor {
  void do_exec(int a) override { std::cout << --a << std::endl; }
};

REGISTER(PlusExecutor);
REGISTER(MinusExecutor);

int main(int argc, char *argv[]) {
  auto var1 =
      static_cast<BaseExecutor *>(SONNIE::GET_INSTANCE_BY_NAME("PlusExecutor"));
  auto var2 = static_cast<BaseExecutor *>(
      SONNIE::GET_INSTANCE_BY_NAME("MinusExecutor"));
  var1->do_exec(1);
  var2->do_exec(1);
  return 0;
}