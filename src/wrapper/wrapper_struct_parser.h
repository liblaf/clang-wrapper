#ifndef WRAPPER_STRUCT_PARSER_H_
#define WRAPPER_STRUCT_PARSER_H_

#include "common/arguments.h"
#include "wrapper_base.h"

class WrapperStructParser : public WrapperBase {
 public:
  virtual int compile_s(Arguments args) override;
};

#endif  // WRAPPER_STRUCT_PARSER_H_
