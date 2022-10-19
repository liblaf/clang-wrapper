#include "path.h"

#if __clang_major__ < 11

#include <string>

#include "llvm/Support/FileSystem.h"
#include "logging.h"

namespace fs {

Path::Path(const std::string& p) : std::string(p) {}

Path& Path::replace_extension(const Path& replacement) {
  while (!(this->empty())) {
    if (this->back() == '.') {
      this->pop_back();
      break;
    } else if (this->back() == '/') {
      break;
    } else {
      this->pop_back();
      continue;
    }
  }
  auto pos = replacement.find_first_not_of('.');
  if (pos == std::string::npos) pos = 0;
  this->push_back('.');
  this->append(replacement.substr(pos));
  return *this;
}

std::string Path::string() const { return *this; }

bool exists(const Path& p) { return llvm::sys::fs::exists(p); }

};  // namespace fs

#endif  // __clang_major__ < 11
