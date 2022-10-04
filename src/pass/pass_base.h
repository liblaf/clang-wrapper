#ifndef PASS_BASE_H_
#define PASS_BASE_H_

#include <memory>
#include <optional>

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

class PassBase {
 public:
  explicit PassBase(llvm::Module& target);
  virtual ~PassBase();

 public:
  virtual bool run_on_module();
  virtual bool run_on_function(llvm::Function& func);
  virtual bool run_on_block(llvm::BasicBlock& block);
  virtual bool run_on_instruction(llvm::Instruction& inst);

 public:
  static std::unique_ptr<llvm::Module> parse_ir_file(
      llvm::StringRef filename, llvm::LLVMContext& context);
  static void write_module(llvm::StringRef filename, llvm::Module& mod);

  template <class PassType = PassBase>
  static bool apply_pass(llvm::StringRef filename) {
    llvm::LLVMContext context;
    auto mod = PassType::parse_ir_file(filename, context);
    auto pass = std::make_unique<PassType>(*mod);
    bool modified = pass->run_on_module();
    if (modified) PassType::write_module(filename, *mod);
    return modified;
  }

 protected:
  llvm::Module& target();

 private:
  llvm::Module& _target;
};

#endif  // PASS_BASE_H_
