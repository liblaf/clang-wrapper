#include "wrapper_base.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <string>
#include <tuple>
#include <vector>

#include "common/arguments.h"
#include "common/env.h"
#include "common/logging.h"
#include "common/path.h"
#include "pass/pass_base.h"

WrapperBase::WrapperBase() {
#ifndef USE_CXX
  this->set_compiler(WrapperBase::get_cc());
#else   // USE_CXX
  this->set_compiler(WrapperBase::get_cxx());
#endif  // USE_CXX
}

WrapperBase::~WrapperBase() {}

auto WrapperBase::prepare_args(Arguments args)
    -> std::tuple<std::vector<Phase>, std::vector<fs::path>, Arguments> {
  auto phases = this->get_phases(args);
  auto inputs = this->get_inputs(phases);
  auto options = this->get_options(args, inputs);
#ifdef USE_CXX
  options = Arguments::remove_stdc(options);
#endif  // USE_CXX
  options = Arguments::suppress_warnings(options);
  return std::make_tuple(phases, inputs, options);
}

std::vector<Phase> WrapperBase::get_phases(Arguments args) {
  args[0] = this->compiler();
  args.insert(args.begin() + 1, "-ccc-print-phases");
  FILE* stream = this->exec_popen(args, /*redirect_stderr=*/true);
  auto output = this->read_all(stream);
  int status = this->exec_pclose(stream);
  if (status != EXIT_SUCCESS) exit(status);
  return Phase::parse_phases(output);
}

std::vector<fs::path> WrapperBase::get_inputs(
    const std::vector<Phase>& phases) {
  std::vector<fs::path> inputs;
  for (const auto& phase : phases) {
    if ((phase.tool == "input") &&
        ((phase.output == "c") || (phase.output == "c++") ||
         (phase.output == "ir"))) {
      assert(fs::exists(phase.input));
      inputs.push_back(phase.input);
    }
  }
  return inputs;
}

Arguments WrapperBase::get_options(const Arguments& args,
                                   const std::vector<fs::path>& inputs) {
  Arguments options;
  for (int i = 1; i < args.size(); ++i) {
    const auto& arg = args[i];
    bool is_input = false;
    for (const auto& input : inputs) {
      if (arg == input) {
        is_input = true;
        break;
      }
    }
    if (is_input) {
      continue;
    }
    options.push_back(arg);
  }
  return options;
}

Arguments WrapperBase::join_args(const Arguments& options,
                                 const std::vector<fs::path>& inputs) {
  Arguments args;
  args.push_back(this->compiler());
  args.insert(args.end(), options.begin(), options.end());
  args.insert(args.end(), inputs.begin(), inputs.end());
  return args;
}

fs::path WrapperBase::generate_ll(Arguments options, const fs::path& input,
                                  std::optional<fs::path> output,
                                  const bool disable_optimize,
                                  const bool enable_debug,
                                  const bool no_opaque_pointers) {
  if (!output) {
    output = input;
    output->replace_extension(".ll");
  }
  if (input == *output) return *output;
  if (disable_optimize) options = Arguments::disable_optimize(options);
  if (enable_debug) options = Arguments::enable_debug(options);
  if (no_opaque_pointers) options = Arguments::no_opaque_pointers(options);
  options.push_back("--assemble");
  options.push_back("-emit-llvm");
  options.push_back("--output");
  options.push_back(*output);
  auto args = this->join_args(options, {input});
  int ret = this->exec_fork(args);
  if (ret) throw ret;
  return *output;
}

fs::path WrapperBase::generate_o(Arguments options, const fs::path& input,
                                 std::optional<fs::path> output,
                                 const bool disable_optimize,
                                 const bool enable_debug) {
  if (!output) {
    output = input;
    output->replace_extension(".o");
  }
  if (input == *output) return *output;
  if (disable_optimize) options = Arguments::disable_optimize(options);
  if (enable_debug) options = Arguments::enable_debug(options);
  options.push_back("--compile");
  options.push_back("--output");
  options.push_back(*output);
  auto args = this->join_args(options, {input});
  int ret = this->exec_fork(args);
  if (ret) throw ret;
  return *output;
}

int WrapperBase::compile_c(int argc, char** argv) {
  auto args = Arguments(argc, argv);
  return this->compile_s(args);
}

int WrapperBase::compile_s(Arguments args) {
  logger().info("compile(): {}", args.command());
  auto [phases, inputs, options] = this->prepare_args(args);
  for (const auto& input : inputs) {
    logger().info("input: {}", input);
  }
  for (const auto& option : options) {
    logger().info("option: {}", option);
  }
  for (auto& input : inputs) {
    try {
      fs::path output = this->generate_ll(options, input);
      bool modified = PassBase::apply_pass(output.string());
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

fs::path WrapperBase::compiler() const { return this->_compiler; }

void WrapperBase::set_compiler(const fs::path& new_compiler) {
  this->_compiler = new_compiler;
}

fs::path WrapperBase::get_cc() {
  fs::path path = get_env("TARGET_CC", TARGET_CC);
  return path;
}

fs::path WrapperBase::get_cxx() {
  fs::path path = get_env("TARGET_CXX", TARGET_CXX);
  return path;
}
