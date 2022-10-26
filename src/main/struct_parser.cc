#include <memory>

#include "wrapper/wrapper_struct_parser.h"

int main(int argc, char** argv) {
  auto wrapper = std::make_unique<WrapperStructParser>();
  int ret = wrapper->compile_c(argc, argv);
  return ret;
}
