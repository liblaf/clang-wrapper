#ifndef JNI_METHOD_DETECTOR_H_
#define JNI_METHOD_DETECTOR_H_

#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"

class JNIMethodDetector {
 public:
  JNIMethodDetector(llvm::Module& target);

  bool is_dynamic_registered(llvm::Function& func);
  bool is_static_registered(llvm::Function& func);
  bool is_jni_method(llvm::Function& func);

 protected:
  bool run_on_module();
  llvm::Module& target();

 private:
  llvm::Module& _target;
};

#endif  // JNI_METHOD_DETECTOR_H_
