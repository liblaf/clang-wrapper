#include <memory>

#include "wrapper/jni/wrapper_interface_counter.h"

int main(int argc, char** argv) {
  auto wrapper = std::make_unique<WrapperInterfaceCounter>();
  int ret = wrapper->compile_c(argc, argv);
  return ret;
}
