#ifndef PASS_CALL_HOOK_H_
#define PASS_CALL_HOOK_H_

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "pass_base.h"
#include "utility/struct_parser.h"

class PassStructParser : public PassBase {
 public:
  explicit PassStructParser(llvm::Module& target);
  virtual ~PassStructParser();

 public:
  virtual bool run_on_instruction(llvm::Instruction& inst) override;
  virtual bool run_on_load_inst(llvm::LoadInst& inst);
  virtual bool run_on_get_element_ptr_inst(llvm::GetElementPtrInst& inst);

 public:
  template <class PassType = PassStructParser>
  static bool apply_pass(llvm::StringRef target_ll) {
    return PassBase::apply_pass<PassType>(target_ll);
  }

 protected:
  StructParser& struct_parser();

 private:
  StructParser* _struct_parser;
};

#endif  // PASS_CALL_HOOK_H_
