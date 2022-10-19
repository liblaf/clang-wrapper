#ifndef EXECUTOR_H_
#define EXECUTOR_H_

#include <cstdio>
#include <string>
#include <vector>

#include "common/arguments.h"

class Executor {
 public:
  static int exec_exec(int argc, char **argv);
  static int exec_exec(const Arguments &args);

  static int exec_fork(int argc, char **argv);
  static int exec_fork(const Arguments &args);

  static FILE *exec_popen(int argc, char **argv,
                          const bool redirect_stderr = false);
  static FILE *exec_popen(const Arguments &args,
                          const bool redirect_stderr = false);
  static std::string read_all(FILE *stream);
  static int exec_pclose(FILE *stream);

  static int exec_system(int argc, char **argv);
  static int exec_system(const Arguments &args);
};

#endif  // EXECUTOR_H_
