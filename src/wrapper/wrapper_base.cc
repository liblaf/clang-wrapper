#include "wrapper_base.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <string>
#include <vector>

#include "common/arguments.h"
#include "common/logging.h"
#include "pass/pass_base.h"

WrapperBase::WrapperBase() {
#ifndef USE_CXX
  this->set_compiler(WrapperBase::get_cc());
#else
  this->set_compiler(WrapperBase::get_cxx());
#endif
}

WrapperBase::~WrapperBase() {}

std::vector<std::string> WrapperBase::prepare_args(
    std::vector<std::string> args) {
  args[0] = this->compiler();
  return args;
}

std::vector<Phase> WrapperBase::get_phases(std::vector<std::string> args) {
  args[0] = this->compiler();
  args.insert(args.begin() + 1, "-ccc-print-phases");
  FILE* stream = this->exec_popen(args, /*redirect_stderr=*/true);
  auto output = this->read_all(stream);
  int status = this->exec_pclose(stream);
  if (status != EXIT_SUCCESS) exit(status);
  return Phase::parse_phases(output);
}

std::vector<std::filesystem::path> WrapperBase::get_inputs(
    const std::vector<Phase>& phases) {
  std::vector<std::filesystem::path> inputs;
  for (const auto& phase : phases) {
    if ((phase.tool == "input") &&
        ((phase.output == "c") || (phase.output == "c++"))) {
      assert(std::filesystem::exists(phase.input));
      inputs.push_back(phase.input);
    }
  }
  return inputs;
}

std::vector<std::string> WrapperBase::get_options(
    const std::vector<std::string>& args,
    const std::vector<std::filesystem::path> inputs) {
  std::vector<std::string> options;
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

std::vector<std::string> WrapperBase::join_args(
    const std::vector<std::string>& options,
    const std::vector<std::filesystem::path>& inputs) {
  std::vector<std::string> args;
  args.push_back(this->compiler());
  args.insert(args.end(), options.begin(), options.end());
  args.insert(args.end(), inputs.begin(), inputs.end());
  return args;
}

std::filesystem::path WrapperBase::generate_ll(
    std::vector<std::string> options, const std::filesystem::path& input,
    std::optional<std::filesystem::path> output, const bool disable_optimize,
    const bool enable_debug) {
  if (!output) {
    output = input;
    output->replace_extension(".ll");
  }
  if (disable_optimize) options = Arguments::disable_optimize(options);
  if (enable_debug) options = Arguments::enable_debug(options);
  options.push_back("--assemble");
  options.push_back("-emit-llvm");
  options.push_back("--output");
  options.push_back(*output);
  auto args = this->join_args(options, {input});
  int ret = this->exec_fork(args);
  if (ret) throw ret;
  return *output;
}

std::filesystem::path WrapperBase::generate_o(
    std::vector<std::string> options, const std::filesystem::path& input,
    std::optional<std::filesystem::path> output, const bool disable_optimize,
    const bool enable_debug) {
  if (!output) {
    output = input;
    output->replace_extension(".ll");
  }
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

int WrapperBase::compile(int argc, char** argv) {
  auto args = Arguments::create_args(argc, argv);
  return this->compile(args);
}

int WrapperBase::compile(std::vector<std::string> args) {
  logger().info("compile(): {}", Arguments::create_command(args));
  args = this->prepare_args(args);
  auto phases = this->get_phases(args);
  for (auto&& phase : phases) {
    logger().info("{}", phase.to_string());
  }
  auto inputs = this->get_inputs(phases);
  for (const auto& input : inputs) {
    logger().info("input: {}", input);
  }
  auto options = this->get_options(args, inputs);
  for (const auto& option : options) {
    logger().info("option: {}", option);
  }
  for (auto& input : inputs) {
    try {
      std::filesystem::path output = this->generate_ll(options, input);
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

std::filesystem::path WrapperBase::compiler() const { return this->_compiler; }

void WrapperBase::set_compiler(const std::filesystem::path& new_compiler) {
  this->_compiler = new_compiler;
}

std::filesystem::path WrapperBase::get_cc() {
  const char* target_cc = std::getenv("TARGET_CC");
  std::filesystem::path path = target_cc ? target_cc : TARGET_CC;
  return path;
}

std::filesystem::path WrapperBase::get_cxx() {
  const char* target_cxx = std::getenv("TARGET_CXX");
  std::filesystem::path path = target_cxx ? target_cxx : TARGET_CXX;
  return path;
}
