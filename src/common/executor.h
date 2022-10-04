#ifndef EXECUTOR_H_
#define EXECUTOR_H_

#include <cstdio>
#include <string>
#include <vector>

class Executor {
 public:
  static int exec_exec(int argc, char **argv);
  static int exec_exec(const std::vector<std::string> &args);

  static int exec_fork(int argc, char **argv);
  static int exec_fork(const std::vector<std::string> &args);

  static FILE *exec_popen(int argc, char **argv,
                          const bool redirect_stderr = false);
  static FILE *exec_popen(const std::vector<std::string> &args,
                          const bool redirect_stderr = false);
  static std::string read_all(FILE *stream);
  static int exec_pclose(FILE *stream);

  static int exec_system(int argc, char **argv);
  static int exec_system(const std::vector<std::string> &args);
};

#endif  // EXECUTOR_H_
