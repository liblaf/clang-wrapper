#include "runtime.h"

#include "stdio.h"

void __before_call(const char *called_function_name) {
  fprintf(stderr, "call %s\n", called_function_name);
}
