#include "wrapper_struct_parser.h"

#include "common/arguments.h"
#include "common/env.h"
#include "common/logging.h"
#include "common/path.h"
#include "common/phase.h"
#include "pass/pass_struct_parser.h"

int WrapperStructParser::compile_s(Arguments args) {
  auto [phases, inputs, options] = this->prepare_args(args);

  for (auto& input : inputs) {
    try {
      fs::path output = this->generate_ll(options, input);
      bool modified = PassStructParser::apply_pass(output.string());
      if (modified) input = output;
    } catch (const int e) {
      logger().error("{} => *.ll failed with exit code {}.", input, e);
    } catch (const std::exception& e) {
      logger().error("instrumentation failed: {}", e.what());
    }
  }

  args = this->join_args(options, inputs);
  int ret = this->exec_fork(args);
  return ret;
}
