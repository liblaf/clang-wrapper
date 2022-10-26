#ifndef WRAPPER_INTERFACE_COUNTER_H_
#define WRAPPER_INTERFACE_COUNTER_H_

#ifndef RUNTIME_HOME
#define RUNTIME_HOME "src/runtime/jni/interface_counter"
#endif  // RUNTIME_HOME

#include "common/arguments.h"
#include "wrapper/wrapper_call_hook.h"

class WrapperInterfaceCounter : public WrapperCallHook {
 public:
  virtual int compile_s(Arguments args) override;
};

#endif  // WRAPPER_INTERFACE_COUNTER_H_
