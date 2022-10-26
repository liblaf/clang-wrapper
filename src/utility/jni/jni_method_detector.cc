#include "jni_method_detector.h"

#include <cassert>

#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "utility/assertion.h"

using namespace llvm;

JNIMethodDetector::JNIMethodDetector(Module& target) : _target(target) {
  auto modified = this->run_on_module();
  assert(modified == false);
}

bool JNIMethodDetector::is_dynamic_registered(llvm::Function& func) {
  if (!function_belong(func, this->target())) return false;
  // TODO
  return false;
}

bool JNIMethodDetector::is_static_registered(llvm::Function& func) {
  if (!function_belong(func, this->target())) return false;
  // TODO
  return false;
}

bool JNIMethodDetector::is_jni_method(Function& func) {
  return this->is_static_registered(func) || this->is_dynamic_registered(func);
}

bool JNIMethodDetector::run_on_module() {
  bool modified = false;
  // TODO
  assert(modified == false);
  return modified;
}

Module& JNIMethodDetector::target() { return this->_target; }
