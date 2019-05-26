// Copyright 2019 Piotrek

#ifndef SERVER_CORE_INSTR_STRUCT_H_
#define SERVER_CORE_INSTR_STRUCT_H_

#include "core/nquad.h"

namespace tin {

// Class that shall be derived for wvery instruction that wants to have memory.
class InstrStruct {
 public:
  static constexpr int NQS = sizeof(NQuad);
  static constexpr int MAGIC = 0;
  static constexpr int INSTR = MAGIC + NQS;
  static constexpr int INSTR2 = INSTR + NQS;

  InstrStruct() {}
  virtual ~InstrStruct() {}
};

}  // namespace tin

#endif  // SERVER_CORE_INSTR_STRUCT_H_
