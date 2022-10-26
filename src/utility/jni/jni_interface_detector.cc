#include "jni_interface_detector.h"

#include <cassert>

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Casting.h"
#include "utility/struct_parser.h"

using namespace llvm;

JNIInterfaceDetector::JNIInterfaceDetector(Module& target)
    : _target(target), _struct_parser(target) {}

StringRef JNIInterfaceDetector::get_interface_name(CallInst& inst) {
  assert(inst.getModule() == &(this->target()));
  auto called_operand = dyn_cast<LoadInst>(inst.getCalledOperand());
  if (!called_operand) return "";
  auto pointer_operand =
      dyn_cast<GetElementPtrInst>(called_operand->getPointerOperand());
  if (!pointer_operand) return "";
  auto name =
      this->struct_parser().get_field_name_from_address(*pointer_operand);
  return name;
}

bool JNIInterfaceDetector::is_interface_call(llvm::CallInst& inst) {
  return this->get_interface_name(inst).empty() == false;
}

llvm::Module& JNIInterfaceDetector::target() { return this->_target; }

StructParser& JNIInterfaceDetector::struct_parser() {
  return this->_struct_parser;
}
