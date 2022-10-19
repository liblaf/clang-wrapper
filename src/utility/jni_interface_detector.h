#ifndef JNI_INTERFACE_DETECTOR_H_
#define JNI_INTERFACE_DETECTOR_H_

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "struct_parser.h"

class JNIInterfaceDetector {
 public:
  explicit JNIInterfaceDetector(llvm::Module& target);

  llvm::StringRef get_interface_name(llvm::CallInst& inst);
  bool is_interface_call(llvm::CallInst& inst);

 protected:
  llvm::Module& target();
  StructParser& struct_parser();

 private:
  llvm::Module& _target;
  StructParser _struct_parser;
};

#endif  // JNI_INTERFACE_DETECTOR_H_
