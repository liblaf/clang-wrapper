#ifndef WRAPPER_CALL_HOOK_H_
#define WRAPPER_CALL_HOOK_H_

#include "wrapper_base.h"

#ifndef RUNTIME_HOME
#define RUNTIME_HOME "src/runtime/call_hook"
#endif  // RUNTIME_HOME

#define RUNTIME_C RUNTIME_HOME "/runtime.c"
#define DECLARATION_C RUNTIME_HOME "/declaration.c"

class WrapperCallHook : public WrapperBase {
 public:
  virtual int compile_s(std::vector<std::string> args) override;
};

#endif  // WRAPPER_CALL_HOOK_H_
