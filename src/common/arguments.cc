#include "arguments.h"

#include <cstring>
#include <regex>
#include <string>
#include <vector>

Arguments::Arguments(const std::vector<std::string>& args)
    : std::vector<std::string>(args) {}

Arguments::Arguments(int argc, char** argv) {
  this->reserve(argc);
  for (int i = 0; i < argc; ++i) {
    this->emplace_back(argv[i]);
  }
}

char** Arguments::argv() const {
  char** argv = new char*[this->size() + 1];
  for (int i = 0; i < this->size(); ++i) {
    argv[i] = new char[this->at(i).size() + 1];
    strcpy(/*dest=*/argv[i], /*src=*/this->at(i).c_str());
  }
  argv[this->size()] = NULL;
  return argv;
}

std::string Arguments::command() const {
  std::string command = "";
  for (int i = 0; i < this->size(); ++i) {
    command += "\"" + escape(this->at(i)) + "\" ";
  }
  return command;
}

void Arguments::free_argv(int argc, char** argv) {
  for (int i = 0; i < argc; ++i) {
    delete[] argv[i];
  }
  delete[] argv;
}

std::string Arguments::command(int argc, char** argv) {
  return Arguments(argc, argv).command();
}

std::string Arguments::escape(std::string str) {
  static const auto PATTERN = std::regex("\\\"|\\$|\\\\|\\`");
  str = std::regex_replace(str, PATTERN, "\\$&");
  return str;
}

Arguments Arguments::disable_optimize(Arguments options) {
  options.push_back("-Xclang");
  options.push_back("-disable-O0-optnone");
  options.push_back("-O0");
  return options;
}

Arguments Arguments::enable_debug(Arguments options) {
  options.push_back("--debug");
  return options;
}

Arguments Arguments::no_opaque_pointers(Arguments options) {
#if __clang_major__ >= 14
  options.push_back("-Xclang");
  options.push_back("-no-opaque-pointers");
#endif  // __clang_major__ >= 14
  return options;
}

Arguments Arguments::suppress_warnings(Arguments options) {
  options.push_back("--no-warnings");
  return options;
}

Arguments Arguments::remove_stdc(Arguments options) {
  static const auto PATTERN = std::regex("-std=c\\d.*");
  for (int i = 0; i < options.size(); ++i) {
    std::smatch res;
    std::regex_match(options[i], res, PATTERN);
    if (res.empty()) continue;
    options.erase(options.begin() + i);
    --i;
  }
  return options;
}
