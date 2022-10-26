#include "pass_base.h"

#include <cassert>
#include <memory>

#include "common/logging.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/SourceMgr.h"
#include "utility/assertion.h"

using namespace llvm;

PassBase::PassBase(Module& target) : _target(target) {}

PassBase::~PassBase() {}

bool PassBase::run_on_module() {
  bool modified = false;
  for (auto& func : this->target()) {
    modified |= this->run_on_function(func);
  }
  return modified;
}

bool PassBase::run_on_function(Function& func) {
  assert_function_belong(func, this->target());
  bool modified = false;
  if (func.getName().contains('.')) return modified;
  for (auto& block : func) {
    modified |= this->run_on_block(block);
  }
  return modified;
}

bool PassBase::run_on_block(BasicBlock& block) {
  assert_block_belong(block, this->target());
  bool modified = false;
  for (auto& inst : block) {
    modified |= this->run_on_instruction(inst);
  }
  return modified;
}

bool PassBase::run_on_instruction(Instruction& inst) {
  assert_instruction_belong(inst, this->target());
  bool modified = false;
  return modified;
}

std::unique_ptr<Module> PassBase::parse_ir_file(llvm::StringRef filename,
                                                llvm::LLVMContext& context) {
  SMDiagnostic err;
  auto mod = parseIRFile(filename, err, context);
  if (!mod) {
    std::string message = "File \"" + err.getFilename().str() + "\", line " +
                          std::to_string(err.getLineNo()) + ", column " +
                          std::to_string(err.getColumnNo()) + "\n";
    message += "  " + err.getLineContents().str() + "\n";
    message += "parseIRFile(): " + err.getMessage().str();
    throw std::runtime_error(message);
  }
  return mod;
}

void PassBase::write_module(llvm::StringRef filename, llvm::Module& mod) {
  if (verifyModule(mod, &logger())) {
    mod.print(logger(), nullptr);
    throw std::runtime_error("Generated module is incorrect!");
  }
  std::error_code ec;
  auto output = raw_fd_ostream(filename, ec, sys::fs::OpenFlags::OF_None);
  mod.print(output, nullptr);
}

Module& PassBase::target() { return this->_target; }
