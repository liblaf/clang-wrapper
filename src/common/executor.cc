#include "executor.h"

#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "arguments.h"
#include "logging.h"

int Executor::exec_exec(int argc, char **argv) {
  logger().info("execvp(): {}", Arguments::create_command(argc, argv));
  int status = execvp(argv[0], argv);
  perror("execvp()");
  return status;
}

int Executor::exec_exec(const std::vector<std::string> &args) {
  int argc = args.size();
  char **argv = Arguments::create_argv(args);
  int status = Executor::exec_exec(argc, argv);
  Arguments::free_argv(argc, argv);
  return status;
}

int Executor::exec_fork(int argc, char **argv) {
  logger().info("fork(): {}", Arguments::create_command(argc, argv));
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork()");
    return pid;
  } else if (pid == 0) {
    // child process
    int return_value = execvp(argv[0], argv);
    perror("execvp()");
    exit(return_value);
  } else {
    // parent process
    int status;
    wait(&status);
    if (status < 0) {
      perror("wait()");
      return status;
    } else {
      logger().info("fork(): exit with code {}.", WEXITSTATUS(status));
      return WEXITSTATUS(status);
    }
  }
}

int Executor::exec_fork(const std::vector<std::string> &args) {
  int argc = args.size();
  char **argv = Arguments::create_argv(args);
  int return_value = Executor::exec_fork(argc, argv);
  Arguments::free_argv(argc, argv);
  return return_value;
}

FILE *Executor::exec_popen(int argc, char **argv, const bool redirect_stderr) {
  const auto &args = Arguments::create_args(argc, argv);
  return Executor::exec_popen(args, redirect_stderr);
}

FILE *Executor::exec_popen(const std::vector<std::string> &args,
                           const bool redirect_stderr) {
  auto command = Arguments::create_command(args);
  if (redirect_stderr) {
    command += " 2>&1";
  }
  logger().info("popen(): {}", command);
  FILE *stream = popen(command.c_str(), "r");
  if (!stream) {
    perror("popen()");
  }
  return stream;
}

std::string Executor::read_all(FILE *stream) {
  static char buffer[BUFSIZ];
  std::string content;
  while (!feof(stream)) {
    int bytes = fread(buffer, 1, BUFSIZ, stream);
    content.append(buffer, bytes);
  }
  return content;
}

int Executor::exec_pclose(FILE *stream) {
  int status = pclose(stream);
  logger().info("pclose(): exit with code {}.", WEXITSTATUS(status));
  return WEXITSTATUS(status);
}

int Executor::exec_system(int argc, char **argv) {
  const auto &args = Arguments::create_args(argc, argv);
  return Executor::exec_system(args);
}

int Executor::exec_system(const std::vector<std::string> &args) {
  auto command = Arguments::create_command(args);
  logger().info("system(): {}", command);
  int status = system(command.c_str());
  if (status < 0) {
    perror("system()");
    return status;
  } else {
    logger().info("system(): exit with code {}.", WEXITSTATUS(status));
    return WEXITSTATUS(status);
  }
}
