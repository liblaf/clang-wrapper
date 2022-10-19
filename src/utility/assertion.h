#ifndef ASSERTION_H_
#define ASSERTION_H_

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"

bool module_equal(llvm::Module& a, llvm::Module& b);
bool function_belong(llvm::Function& func, llvm::Module& mod);
bool block_belong(llvm::BasicBlock& block, llvm::Module& mod);
bool instruction_belong(llvm::Instruction& inst, llvm::Module& mod);

void assert_module_equal(llvm::Module& a, llvm::Module& b);
void assert_function_belong(llvm::Function& func, llvm::Module& mod);
void assert_block_belong(llvm::BasicBlock& block, llvm::Module& mod);
void assert_instruction_belong(llvm::Instruction& inst, llvm::Module& mod);

#endif  // ASSERTION_H_
