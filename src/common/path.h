#ifndef PATH_H_
#define PATH_H_

#if __clang_major__ >= 11

#include <filesystem>

namespace fs = std::filesystem;

#else

#include <string>

namespace fs {

// avoid bugs with std::filesystem of clang-9

class Path : public std::string {
 public:
  using std::string::string;

  Path(const std::string& p);

  Path& replace_extension(const Path& replacement = Path());
  std::string string() const;
};

bool exists(const Path& p);

typedef Path path;

};  // namespace fs

#endif  // __clang_major__ >= 11

#endif  // PATH_H_
