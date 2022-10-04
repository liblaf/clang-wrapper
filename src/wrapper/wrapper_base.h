#ifndef WRAPPER_BASE_H_
#define WRAPPER_BASE_H_

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "common/executor.h"
#include "common/phase.h"

#ifndef TARGET_CC
#define TARGET_CC "clang"
#endif  // TARGET_CC

#ifndef TARGET_CXX
#define TARGET_CXX "clang++"
#endif  // TARGET_CXX

// #define USE_CXX

class WrapperBase : public Executor {
 public:
  explicit WrapperBase();
  virtual ~WrapperBase();

 public:
  virtual std::vector<std::string> prepare_args(std::vector<std::string> args);

  std::vector<Phase> get_phases(std::vector<std::string> args);
  std::vector<std::filesystem::path> get_inputs(
      const std::vector<Phase>& phases);
  std::vector<std::string> get_options(
      const std::vector<std::string>& args,
      const std::vector<std::filesystem::path> inputs);
  std::vector<std::string> join_args(
      const std::vector<std::string>& options,
      const std::vector<std::filesystem::path>& inputs);

  virtual std::filesystem::path generate_ll(
      std::vector<std::string> options, const std::filesystem::path& input,
      std::optional<std::filesystem::path> output = {},
      const bool disable_optimize = true, const bool enable_debug = true);
  virtual std::filesystem::path generate_o(
      std::vector<std::string> options, const std::filesystem::path& input,
      std::optional<std::filesystem::path> output = {},
      const bool disable_optimize = true, const bool enable_debug = true);

  virtual int compile(int argc, char** argv);
  virtual int compile(std::vector<std::string> args);

 public:
  std::filesystem::path compiler() const;
  void set_compiler(const std::filesystem::path& new_compiler);

 protected:
  static std::filesystem::path get_cc();
  static std::filesystem::path get_cxx();

 private:
  std::filesystem::path _compiler;
};

#endif  // WRAPPER_BASE_H_
