#include "assertion.h"

#include <cassert>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"

bool module_equal(llvm::Module& a, llvm::Module& b) {
#if __clang_major__ >= 11
  return a.getModuleIdentifier() == b.getModuleIdentifier();
#else
  return true;
#endif
}

bool function_belong(llvm::Function& func, llvm::Module& mod) {
  return module_equal(*(func.getParent()), mod);
}

bool block_belong(llvm::BasicBlock& block, llvm::Module& mod) {
  return module_equal(*(block.getModule()), mod);
}

bool instruction_belong(llvm::Instruction& inst, llvm::Module& mod) {
  return module_equal(*(inst.getModule()), mod);
}

void assert_module_equal(llvm::Module& a, llvm::Module& b) {
  assert(module_equal(a, b));
}

void assert_function_belong(llvm::Function& func, llvm::Module& mod) {
  assert(function_belong(func, mod));
}

void assert_block_belong(llvm::BasicBlock& block, llvm::Module& mod) {
  assert(block_belong(block, mod));
}

void assert_instruction_belong(llvm::Instruction& inst, llvm::Module& mod) {
  assert(instruction_belong(inst, mod));
}
