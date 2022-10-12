#ifndef STRUCT_PARSER_H_
#define STRUCT_PARSER_H_

#include <map>
#include <string>
#include <vector>

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"

class StructParser {
 public:
  explicit StructParser(llvm::Module& target);

  llvm::StringRef get_field_name(llvm::StructType& type, const int index);
  llvm::StringRef get_field_name_from_address(llvm::GetElementPtrInst& addr);
  llvm::StringRef get_field_name_from_value(llvm::LoadInst& val);
  static int get_gep_index(llvm::GetElementPtrInst& inst);
  int get_member_offset_in_bits(llvm::StructType& type, const int index);

 protected:
  bool run_on_module();
  bool run_on_function(llvm::Function& func);
  bool run_on_block(llvm::BasicBlock& block);
  bool run_on_instruction(llvm::Instruction& inst);
  bool run_on_dbg_declare_inst(llvm::DbgDeclareInst& inst);

  void probe_type(llvm::Type* type, llvm::DIType* di_type);

  llvm::Module& target();

  static llvm::DIType* get_base_type(llvm::DIType* type);

 private:
  llvm::Module& _target;
  std::map<llvm::Type*, llvm::DIType*> _type_map;
};

#endif  // STRUCT_PARSER_H_
