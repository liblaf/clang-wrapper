#include "location.h"

#include <sstream>
#include <string>

#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

std::string get_inst_loc(Instruction& inst) {
  std::string str;
  auto oss = raw_string_ostream(str);
  auto debug_loc = inst.getDebugLoc();
  if (debug_loc) {
    int line = debug_loc.getLine();
    int col = debug_loc.getCol();
    auto dir = debug_loc->getDirectory();
    auto file = debug_loc->getFilename();
    oss << dir.str() << "/" << file.str() << ":" << line << ":" << col;
  } else {
    auto mod = inst.getModule()->getName();
    auto func = inst.getFunction()->getName();
    auto opcode = inst.getOpcodeName();
    oss << mod.str() << ":" << func.str() << ":" << opcode;
  }
  return oss.str();
}
