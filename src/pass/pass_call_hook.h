#ifndef PASS_CALL_HOOK_H_
#define PASS_CALL_HOOK_H_

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "pass_base.h"

#ifndef HOOK_NAME
#define HOOK_NAME "__before_call"
#endif  // HOOK_NAME

class PassCallHook : public PassBase {
 public:
  explicit PassCallHook(llvm::Module& target, llvm::Module& declaration);

 public:
  virtual bool run_on_instruction(llvm::Instruction& inst) override;
  virtual bool run_on_call_inst(llvm::CallInst& inst);

 public:
  template <class PassType = PassCallHook>
  static bool apply_pass(llvm::StringRef target_ll,
                         llvm::StringRef declaration_ll) {
    llvm::LLVMContext context;
    auto target = PassType::parse_ir_file(target_ll, context);
    auto declaration = PassType::parse_ir_file(declaration_ll, context);
    auto pass = std::make_unique<PassType>(*target, *declaration);
    bool modified = pass->run_on_module();
    if (modified) PassType::write_module(target_ll, *target);
    return modified;
  }

 protected:
  virtual void prepare_hooks();
  llvm::Module& declaration();

 private:
  llvm::Module& _declaration;
};

#endif  // PASS_CALL_HOOK_H_
