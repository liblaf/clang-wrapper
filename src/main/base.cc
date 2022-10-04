#include <memory>
#include <string>
#include <vector>

#include "wrapper/wrapper_base.h"

int main(int argc, char** argv) {
  auto wrapper = std::make_unique<WrapperBase>();
  int ret = wrapper->compile_c(argc, argv);
  return ret;
}
