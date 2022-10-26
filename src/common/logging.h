#ifndef LOGGING_H_
#define LOGGING_H_

#include <string>
#include <system_error>

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "path.h"

#ifndef LOG_PATH
#define LOG_PATH "/dev/stderr"
#endif  // LOG_PATH

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif  // LOG_LEVEL

#define LOG_LEVEL_OFF 100
#define LOG_LEVEL_CRITICAL 50
#define LOG_LEVEL_ERROR 40
#define LOG_LEVEL_WARNING 30
#define LOG_LEVEL_INFO 20
#define LOG_LEVEL_DEBUG 10
#define LOG_LEVEL_NOTSET 0

class Logger : public llvm::raw_fd_ostream {
 public:
  explicit Logger(llvm::StringRef filename, std::error_code& ec,
                  llvm::sys::fs::OpenFlags flags);
  ~Logger();

  template <class... Args>
  Logger& critical(const char* fmt, Args&&... args) {
    if (this->log_level() > LOG_LEVEL_CRITICAL) return *this;
    this->change_color(llvm::raw_ostream::Colors::RED, /*bold=*/true);
    this->log("{}", Logger::get_prompt("CRITICAL"));
    this->log(fmt, args...);
    this->log("\n");
    this->reset_color();
    return *this;
  }

  template <class... Args>
  Logger& error(const char* fmt, Args&&... args) {
    if (this->log_level() > LOG_LEVEL_ERROR) return *this;
    this->change_color(llvm::raw_ostream::Colors::RED);
    this->log("{}", Logger::get_prompt("ERROR"));
    this->log(fmt, args...);
    this->log("\n");
    this->reset_color();
    return *this;
  }

  template <class... Args>
  Logger& warning(const char* fmt, Args&&... args) {
    if (this->log_level() > LOG_LEVEL_WARNING) return *this;
    this->change_color(llvm::raw_ostream::Colors::YELLOW, /*bold=*/true);
    this->log("{}", Logger::get_prompt("WARNING"));
    this->log(fmt, args...);
    this->log("\n");
    this->reset_color();
    return *this;
  }

  template <class... Args>
  Logger& info(const char* fmt, Args&&... args) {
    if (this->log_level() > LOG_LEVEL_INFO) return *this;
    this->change_color(llvm::raw_ostream::Colors::BLUE);
    this->log("{}", Logger::get_prompt("INFO"));
    this->log(fmt, args...);
    this->log("\n");
    this->reset_color();
    return *this;
  }

  template <class... Args>
  Logger& debug(const char* fmt, Args&&... args) {
    if (this->log_level() > LOG_LEVEL_DEBUG) return *this;
    this->log("{}", Logger::get_prompt("DEBUG"));
    this->log(fmt, args...);
    this->log("\n");
    return *this;
  }

  fs::path filename() const;
  int log_level() const;
  void set_log_level(const int new_log_level = LOG_LEVEL_NOTSET);

  static fs::path get_log_path();
  static std::string time();

 protected:
  template <class T, class... Args>
  Logger& log(const char* fmt, T&& value, Args&&... args) {
    for (; (*fmt) != '\0'; ++fmt) {
      if ((*fmt) == '{') {
        ++fmt;
        switch (*fmt) {
          case '{': {
            (*this) << '{';
            continue;
          }
          case '}': {
            (*this) << value;
            this->log(fmt + 1, args...);
            return *this;
          }
          default: {
            throw std::invalid_argument("Invalid format string");
          }
        }
      } else {
        (*this) << (*fmt);
      }
    }
    return *this;
  }

  Logger& log(const char* fmt);

  Logger& change_color(llvm::raw_ostream::Colors color, bool bold = false,
                       bool background = false);
  Logger& reset_color();

  static std::string get_prompt(const std::string& type);

 private:
  fs::path _filename;
  int _log_level = LOG_LEVEL_NOTSET;
};

Logger& logger();

#endif  // LOGGING_H_
