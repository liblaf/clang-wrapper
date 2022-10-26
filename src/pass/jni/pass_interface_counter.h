#ifndef PASS_INTERFACE_COUNTER_H_
#define PASS_INTERFACE_COUNTER_H_

#ifndef HOOK_NAME
#define HOOK_NAME "__before_interface_call"
#endif  // HOOK_NAME

#include "llvm/IR/Module.h"
#include "pass/pass_call_hook.h"
#include "utility/jni/jni_interface_detector.h"

class PassInterfaceCounter : public PassCallHook {
 public:
  explicit PassInterfaceCounter(llvm::Module& target,
                                llvm::Module& declaration);

  virtual bool run_on_call_inst(llvm::CallInst& inst) override;

  template <class PassType = PassInterfaceCounter>
  static bool apply_pass(llvm::StringRef target_ll,
                         llvm::StringRef declaration_ll) {
    return PassCallHook::apply_pass<PassType>(target_ll, declaration_ll);
  }

 protected:
  JNIInterfaceDetector& interface_detector();

 private:
  JNIInterfaceDetector _interface_detector;
};

#endif  // PASS_INTERFACE_COUNTER_H_
