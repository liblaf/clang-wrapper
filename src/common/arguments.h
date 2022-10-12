#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

#include <string>
#include <vector>

class Arguments {
 public:
  static std::vector<std::string> create_args(int argc, char** argv);

  static char** create_argv(const std::vector<std::string>& args);
  static void free_argv(int argc, char** argv);

  static std::string create_command(int argc, char** argv);
  static std::string create_command(const std::vector<std::string>& args);
  static std::string escape(std::string str);

 public:
  static std::vector<std::string> disable_optimize(
      std::vector<std::string> options);
  static std::vector<std::string> enable_debug(
      std::vector<std::string> options);
  static std::vector<std::string> no_opaque_pointers(
      std::vector<std::string> options);
};

#endif  // ARGUMENTS_H_
