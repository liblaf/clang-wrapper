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
  explicit PassBase(llvm::Module& mod);
  virtual ~PassBase();

 public:
  virtual bool run_on_module();
  virtual bool run_on_function(llvm::Function& func);
  virtual bool run_on_block(llvm::BasicBlock& block);
  virtual bool run_on_instruction(llvm::Instruction& inst);

 public:
  llvm::Module& target();

 public:
  static std::unique_ptr<llvm::Module> parse_ir_file(
      llvm::StringRef filename, llvm::LLVMContext& context);
  static void write_module(llvm::StringRef filename, llvm::Module& mod);
  static bool apply_pass(llvm::StringRef filename);

 public:
  llvm::Module& _target;
};

#endif  // PASS_BASE_H_
