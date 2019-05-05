// Copyright 2019 Piotrek

#ifndef SERVER_CORE_INSTR_STRUCT_H_
#define SERVER_CORE_INSTR_STRUCT_H_

namespace tin {

// Class that shall be derived for wvery instruction that wants to have memory.
class InstrStruct {
 public:
  InstrStruct() {}
  virtual ~InstrStruct() {}
};

}  // namespace tin

#endif  // SERVER_CORE_INSTR_STRUCT_H_
