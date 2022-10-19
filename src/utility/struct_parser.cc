#include "struct_parser.h"

#include <cassert>
#include <optional>
#include <vector>

#include "llvm/ADT/StringRef.h"
#include "llvm/BinaryFormat/Dwarf.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Module.h"

using namespace llvm;

StructParser::StructParser(Module& target) : _target(target) {
  this->run_on_module();
}

StringRef StructParser::get_field_name(StructType& type, const int index) {
  if (index < 0) return "";
  int offset_in_bits = this->get_member_offset_in_bits(type, index);
  auto iter = this->_type_map.find(&type);
  if (iter == this->_type_map.end()) return "";
  auto di_type = cast<DICompositeType>(iter->second);
  auto elements = di_type->getElements();
  for (int i = 0; i < elements.size(); ++i) {
    auto member = cast<DIDerivedType>(elements[i]);
    if (member->getOffsetInBits() == offset_in_bits) {
      return member->getName();
    }
  }
  return "";
}

StringRef StructParser::get_field_name_from_address(GetElementPtrInst& addr) {
  auto source_element_type = addr.getSourceElementType();
  if (!source_element_type->isStructTy()) return "";
  auto struct_type = cast<StructType>(source_element_type);
  return this->get_field_name(*struct_type, StructParser::get_gep_index(addr));
}

StringRef StructParser::get_field_name_from_value(LoadInst& val) {
  auto addr = val.getPointerOperand();
  if (!isa<GetElementPtrInst>(addr)) return "";
  auto get_element_ptr_inst = cast<GetElementPtrInst>(addr);
  return this->get_field_name_from_address(*get_element_ptr_inst);
}

int StructParser::get_gep_index(GetElementPtrInst& inst) {
  if (inst.getNumIndices() != 2) return -1;
  auto use = inst.idx_begin() + (inst.getNumIndices() - 1);
  if (!use) return -1;
  auto constant_int = dyn_cast_or_null<ConstantInt>(use->get());
  if (!constant_int) return -1;
  return constant_int->getSExtValue();
}

int StructParser::get_member_offset_in_bits(StructType& type, const int index) {
  auto data_layout = this->target().getDataLayout();
  auto struct_layout = data_layout.getStructLayout(&type);
  return struct_layout->getElementOffsetInBits(index);
}

bool StructParser::run_on_module() {
  bool modified = false;
  for (auto& func : this->target()) {
    modified |= this->run_on_function(func);
  }
  assert(modified == false);
  return modified;
}

bool StructParser::run_on_function(Function& func) {
  bool modified = false;
  for (auto& block : func) {
    modified |= this->run_on_block(block);
  }
  assert(modified == false);
  return modified;
}

bool StructParser::run_on_block(BasicBlock& block) {
  bool modified = false;
  for (auto& inst : block) {
    modified |= this->run_on_instruction(inst);
  }
  assert(modified == false);
  return modified;
}

bool StructParser::run_on_instruction(Instruction& inst) {
  bool modified = false;
  if (isa<DbgDeclareInst>(&inst)) {
    auto dbg_declare_inst = cast<DbgDeclareInst>(&inst);
    modified |= this->run_on_dbg_declare_inst(*dbg_declare_inst);
  }
  assert(modified == false);
  return modified;
}

bool StructParser::run_on_dbg_declare_inst(DbgDeclareInst& inst) {
  bool modified = false;
  auto addr = inst.getAddress();
  auto local_variable = inst.getVariable();
  auto expression = inst.getExpression();
  if (expression->getNumElements() > 0) return modified;
  auto di_type = StructParser::get_base_type(local_variable->getType());
  auto pointer_type = cast<PointerType>(addr->getType());
  this->probe_type(
#if __clang_major__ >= 14
      pointer_type->getNonOpaquePointerElementType(),
#else
      pointer_type->getPointerElementType(),
#endif
      di_type);
  assert(modified == false);
  return modified;
}

void StructParser::probe_type(Type* type, DIType* di_type) {
  if (!type) return;
  di_type = StructParser::get_base_type(di_type);
  if (!di_type) return;
  if (this->_type_map.find(type) != this->_type_map.end()) return;
  this->_type_map[type] = di_type;
  switch (di_type->getTag()) {
    case dwarf::DW_TAG_null: {
      break;
    }
    case dwarf::DW_TAG_pointer_type: {
      assert(type->isPointerTy());
      auto pointer_type = cast<PointerType>(type);
      auto debug_info_derived_type = cast<DIDerivedType>(di_type);
      this->probe_type(
#if __clang_major__ >= 14
          pointer_type->getNonOpaquePointerElementType(),
#else
          pointer_type->getPointerElementType(),
#endif
          debug_info_derived_type->getBaseType());
      break;
    }
    case dwarf::DW_TAG_structure_type: {
      assert(type->isStructTy());
      auto struct_type = cast<StructType>(type);
      auto debug_info_composite_type = cast<DICompositeType>(di_type);
      auto elements = debug_info_composite_type->getElements();
      int i = 0, j = 0;
      auto data_layout = this->target().getDataLayout();
      auto struct_layout = data_layout.getStructLayout(struct_type);
      if (!struct_layout) break;
      while (i < struct_type->getStructNumElements() && j < elements.size()) {
        auto member = dyn_cast<DIDerivedType>(elements[j]);
        if (!member) {
          ++j;
          continue;
        }
        if (struct_layout->getElementOffsetInBits(i) <
            member->getOffsetInBits()) {
          ++i;
        } else if (struct_layout->getElementOffsetInBits(i) >
                   member->getOffsetInBits()) {
          ++j;
        } else {
          this->probe_type(struct_type->getStructElementType(i),
                           StructParser::get_base_type(member));
          ++i;
          ++j;
        }
      }
      break;
    }
    default: {
      break;
    }
  }
}

Module& StructParser::target() { return this->_target; }

DIType* StructParser::get_base_type(DIType* type) {
  if (!type) return type;
  switch (type->getTag()) {
    case dwarf::DW_TAG_member:
    case dwarf::DW_TAG_typedef:
    case dwarf::DW_TAG_const_type: {
      auto derived_type = cast<DIDerivedType>(type);
      return StructParser::get_base_type(derived_type->getBaseType());
    }
    default: {
      return type;
    }
  }
}
