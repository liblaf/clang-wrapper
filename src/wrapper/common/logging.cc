#include "logging.h"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <system_error>

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

static std::unique_ptr<Logger> _logger = nullptr;

int Logger::log_level() const { return this->_log_level; }

void Logger::set_log_level(const int new_log_level) {
  if (new_log_level == LOG_LEVEL_NOTSET) {
    const char* log_level_str = std::getenv("LOG_LEVEL");
    if (log_level_str) {
      this->_log_level = std::atoi(log_level_str);
    } else {
      this->_log_level = LOG_LEVEL;
    }
  } else {
    this->_log_level = new_log_level;
  }
}

const char* Logger::get_log_path() {
  const char* log_path = std::getenv("LOG_PATH");
  if (log_path) {
    return log_path;
  } else {
    return LOG_PATH;
  }
}

Logger& Logger::log(const char* fmt) {
  (*this) << fmt;
  return *this;
}

Logger& Logger::change_color(raw_ostream::Colors color, bool bold,
                             bool background) {
  if (this->has_colors()) {
    this->changeColor(color, bold, background);
  }
  return *this;
}

Logger& Logger::reset_color() {
  if (this->has_colors()) {
    this->resetColor();
  }
  return *this;
}

std::string Logger::get_prompt(const std::string& type) {
  std::ostringstream oss;
  auto now = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::high_resolution_clock::to_time_t(now);
  auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
  std::chrono::nanoseconds nanoseconds = now - seconds;
  oss << '[' << std::put_time(std::localtime(&t), "%F %T") << "."
      << std::setw(9) << std::setfill('0') << nanoseconds.count() << "] "
      << std::setw(10) << std::setfill(' ') << ('[' + type + ']') << ' ';
  return oss.str();
}

Logger& logger() { return *_logger; }

static void __attribute__((constructor)) _logger_init() {
  if (_logger) return;
  std::error_code ec;
  _logger = std::make_unique<Logger>(Logger::get_log_path(), ec,
                                     sys::fs::OpenFlags::OF_Append);
  logger().set_log_level();
}

static void __attribute__((destructor)) _logger_exit() {}
