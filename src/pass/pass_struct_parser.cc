#include "pass_struct_parser.h"

#include <cassert>

#include "common/logging.h"
#include "llvm/IR/Instructions.h"
#include "pass/pass_base.h"
#include "utility/struct_parser.h"

using namespace llvm;

PassStructParser::PassStructParser(llvm::Module& target) : PassBase(target) {
  this->_struct_parser = new StructParser(target);
}

PassStructParser::~PassStructParser() { delete this->_struct_parser; }

bool PassStructParser::run_on_instruction(llvm::Instruction& inst) {
  bool modified = false;
  if (isa<LoadInst>(&inst)) {
    auto load_inst = cast<LoadInst>(&inst);
    modified |= this->run_on_load_inst(*load_inst);
  } else if (isa<GetElementPtrInst>(&inst)) {
    auto get_element_ptr_inst = cast<GetElementPtrInst>(&inst);
    modified |= this->run_on_get_element_ptr_inst(*get_element_ptr_inst);
  }
  assert(modified == false);
  return modified;
}

bool PassStructParser::run_on_load_inst(LoadInst& inst) {
  bool modified = false;
  auto name = this->struct_parser().get_field_name_from_value(inst);
  if (name.empty()) return modified;
  logger().info("{}: {}", name, inst);
  return modified;
}

bool PassStructParser::run_on_get_element_ptr_inst(GetElementPtrInst& inst) {
  bool modified = false;
  auto name = this->struct_parser().get_field_name_from_address(inst);
  if (name.empty()) return modified;
  logger().info("{}: {}", name, inst);
  return modified;
}

StructParser& PassStructParser::struct_parser() {
  return *(this->_struct_parser);
}
