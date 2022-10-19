#ifndef LOCATION_H_
#define LOCATION_H_

#include <string>

#include "llvm/IR/Instruction.h"

std::string get_inst_loc(llvm::Instruction& inst);

#endif  // LOCATION_H_
