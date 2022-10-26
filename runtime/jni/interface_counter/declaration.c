#include "declaration.h"

#include "runtime.h"

#ifndef NULL
#define NULL 0
#endif  // NULL

void __foo() { __before_interface_call(NULL, NULL); }
