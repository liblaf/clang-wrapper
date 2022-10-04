#include "env.h"

#include <cstdlib>

const char* get_env(const char* name, const char* default_value) {
  const char* value = std::getenv(name);
  if (value) {
    return value;
  } else {
    return default_value;
  }
}
