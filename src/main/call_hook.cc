#include <memory>

#include "wrapper/wrapper_call_hook.h"

int main(int argc, char** argv) {
  auto wrapper = std::make_unique<WrapperCallHook>();
  int ret = wrapper->compile_c(argc, argv);
  return ret;
}
