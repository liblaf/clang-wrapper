#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

#include <string>
#include <vector>

class Arguments : public std::vector<std::string> {
 public:
  using std::vector<std::string>::vector;
  Arguments(const std::vector<std::string>& args);
  Arguments(int argc, char** argv);

  char** argv() const;
  std::string command() const;

  static void free_argv(int argc, char** argv);
  static std::string command(int argc, char** argv);
  static std::string escape(std::string str);

  static Arguments disable_optimize(Arguments options);
  static Arguments enable_debug(Arguments options);
  static Arguments no_opaque_pointers(Arguments options);
  static Arguments suppress_warnings(Arguments options);
};

#endif  // ARGUMENTS_H_
