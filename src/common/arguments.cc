#include "arguments.h"

#include <cstring>
#include <regex>
#include <string>
#include <vector>

std::vector<std::string> Arguments::create_args(int argc, char** argv) {
  std::vector<std::string> args;
  args.reserve(argc);
  for (int i = 0; i < argc; ++i) {
    args.emplace_back(argv[i]);
  }
  return args;
}

char** Arguments::create_argv(const std::vector<std::string>& args) {
  char** argv = new char*[args.size() + 1];
  for (int i = 0; i < args.size(); ++i) {
    argv[i] = new char[args[i].size() + 1];
    strcpy(/*dest=*/argv[i], /*src=*/args[i].c_str());
  }
  argv[args.size()] = NULL;
  return argv;
}

void Arguments::free_argv(int argc, char** argv) {
  for (int i = 0; i < argc; ++i) {
    delete[] argv[i];
  }
  delete[] argv;
}

std::string Arguments::create_command(int argc, char** argv) {
  return Arguments::create_command(Arguments::create_args(argc, argv));
}

std::string Arguments::create_command(const std::vector<std::string>& args) {
  std::string command = "";
  for (int i = 0; i < args.size(); ++i) {
    command += "\"" + escape(args[i]) + "\" ";
  }
  return command;
}

std::string Arguments::escape(std::string str) {
  static const auto PATTERN = std::regex("\\\"|\\$|\\\\|\\`");
  str = std::regex_replace(str, PATTERN, "\\$&");
  return str;
}

std::vector<std::string> Arguments::disable_optimize(
    std::vector<std::string> options) {
  options.push_back("-Xclang");
  options.push_back("-disable-O0-optnone");
  options.push_back("-O0");
  return options;
}

std::vector<std::string> Arguments::enable_debug(
    std::vector<std::string> options) {
  options.push_back("--debug");
  return options;
}
