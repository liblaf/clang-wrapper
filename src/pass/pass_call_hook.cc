#include "pass_call_hook.h"

#include <cassert>
#include <sstream>
#include <string>

#include "llvm/ExecutionEngine/Orc/IndirectionUtils.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "pass/pass_base.h"
#include "utility/assertion.h"

using namespace llvm;

PassCallHook::PassCallHook(Module& target, Module& declaration)
    : PassBase(target), _declaration(declaration) {
  this->prepare_hooks();
}

bool PassCallHook::run_on_instruction(Instruction& inst) {
  assert_instruction_belong(inst, this->target());
  bool modified = false;
  if (isa<CallInst>(&inst)) {
    modified |= this->run_on_call_inst(*cast<CallInst>(&inst));
  }
  return modified;
}

bool PassCallHook::run_on_call_inst(CallInst& inst) {
  assert_instruction_belong(inst, this->target());
  auto called_func = inst.getCalledFunction();
  auto called_func_name = called_func->getName();
  auto debug_loc = inst.getDebugLoc();
  std::ostringstream oss;
  if (debug_loc) {
    int line = debug_loc.getLine();
    int col = debug_loc.getCol();
    oss << line << ":" << col << ":" << called_func_name.str();
  } else {
    oss << called_func_name.str();
  }
  auto builder = IRBuilder<>(&inst);
  auto loc = builder.CreateGlobalStringPtr(oss.str());
  auto hook_func = this->target().getFunction(HOOK_NAME);
  auto hook_call = builder.CreateCall(hook_func, {loc});
  assert(hook_call);
  return true;
}

void PassCallHook::prepare_hooks() {
  auto origin = this->declaration().getFunction(HOOK_NAME);
  auto clone = orc::cloneFunctionDecl(this->target(), *origin);
  assert(clone);
}

Module& PassCallHook::declaration() { return this->_declaration; }
