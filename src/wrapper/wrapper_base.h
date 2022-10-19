#ifndef WRAPPER_BASE_H_
#define WRAPPER_BASE_H_

#include <filesystem>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "common/arguments.h"
#include "common/executor.h"
#include "common/path.h"
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
  virtual auto prepare_args(Arguments args)
      -> std::tuple<std::vector<Phase>, std::vector<fs::path>, Arguments>;

  std::vector<Phase> get_phases(Arguments args);
  std::vector<fs::path> get_inputs(const std::vector<Phase>& phases);
  Arguments get_options(const Arguments& args,
                        const std::vector<fs::path>& inputs);
  Arguments join_args(const Arguments& options,
                      const std::vector<fs::path>& inputs);

  virtual fs::path generate_ll(Arguments options, const fs::path& input,
                               std::optional<fs::path> output = std::nullopt,
                               const bool disable_optimize = true,
                               const bool enable_debug = true,
                               const bool no_opaque_pointers = true);
  virtual fs::path generate_o(Arguments options, const fs::path& input,
                              std::optional<fs::path> output = std::nullopt,
                              const bool disable_optimize = true,
                              const bool enable_debug = true);

  virtual int compile_c(int argc, char** argv);
  virtual int compile_s(Arguments args);

 public:
  fs::path compiler() const;
  void set_compiler(const fs::path& new_compiler);

 protected:
  static fs::path get_cc();
  static fs::path get_cxx();

 private:
  fs::path _compiler;
};

#endif  // WRAPPER_BASE_H_
