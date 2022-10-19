#include "wrapper_call_hook.h"

#include "common/arguments.h"
#include "common/env.h"
#include "common/logging.h"
#include "common/path.h"
#include "common/phase.h"
#include "pass/pass_call_hook.h"

int WrapperCallHook::compile_s(Arguments args) {
  auto [phases, inputs, options] = this->prepare_args(args);

  auto has_link_stage = Phase::has_link_stage(phases);
  auto declaration_c = get_env("DECLARATION_C", DECLARATION_C);
  auto declaration_ll = this->generate_ll(options, declaration_c);
  auto runtime_c = get_env("RUNTIME_C", RUNTIME_C);
  auto runtime_o = this->generate_o(options, runtime_c);

  for (auto& input : inputs) {
    try {
      fs::path output = this->generate_ll(options, input);
      bool modified =
          PassCallHook::apply_pass(output.string(), declaration_ll.string());
      if (modified) input = output;
    } catch (const int e) {
      logger().error("{} => *.ll failed with exit code {}.", input, e);
    } catch (const std::exception& e) {
      logger().error("instrumentation failed: {}", e.what());
    }
  }

  if (has_link_stage) inputs.push_back(runtime_o);
  args = this->join_args(options, inputs);
  int ret = this->exec_fork(args);
  return ret;
}
