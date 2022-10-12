#ifndef WRAPPER_STRUCT_PARSER_H_
#define WRAPPER_STRUCT_PARSER_H_

#include "wrapper_base.h"

class WrapperStructParser : public WrapperBase {
 public:
  virtual int compile_s(std::vector<std::string> args) override;
};

#endif  // WRAPPER_STRUCT_PARSER_H_
