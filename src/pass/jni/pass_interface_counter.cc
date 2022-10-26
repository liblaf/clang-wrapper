#include "pass_interface_counter.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "pass/pass_call_hook.h"
#include "utility/assertion.h"
#include "utility/location.h"

using namespace llvm;

PassInterfaceCounter::PassInterfaceCounter(Module& target, Module& declaration)
    : PassCallHook(target, declaration), _interface_detector(target) {}

bool PassInterfaceCounter::run_on_call_inst(CallInst& inst) {
  assert_instruction_belong(inst, this->target());
  bool modified = false;
  if (this->interface_detector().is_interface_call(inst) == false)
    return modified;
  auto called_func_name = this->interface_detector().get_interface_name(inst);
  auto builder = IRBuilder<>(&inst);
  auto loc = builder.CreateGlobalStringPtr(get_inst_loc(inst));
  auto called_func_name_val = builder.CreateGlobalStringPtr(called_func_name);
  auto hook_func = this->target().getFunction(HOOK_NAME);
  auto hook_call = builder.CreateCall(hook_func, {loc, called_func_name_val});
  modified = true;
  assert(hook_call);
  return modified;
}

JNIInterfaceDetector& PassInterfaceCounter::interface_detector() {
  return this->_interface_detector;
}
