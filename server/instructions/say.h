// Copyright 2019 TIN

#ifndef SERVER_INSTRUCTIONS_SAY_H_
#define SERVER_INSTRUCTIONS_SAY_H_

#include <iostream>
#include <string>
#include <algorithm>

#include "core/instr_struct.h"
#include "core/nquad.h"
#include "core/server.h"

namespace tin {
class Say : public InstrStruct {
 public:
  static constexpr int START = INSTR + NQS;
  static constexpr int32_t LEN_CUT = 2048;

  Say() : len_is_read_(false) {}
  virtual ~Say() {}
  virtual int Fn(Server *, SocketStuff *, World *, MsgPushFn);

 private:
  constexpr int MsgLen_() const {return START;}
  constexpr int Msg_() const {return MsgLen_() + NQS;}
  constexpr int End_() const {return Msg_() + len_;}

  constexpr int32_t Len_() const {
    return len_.Int();
  }

  Username un_;
  NQuad len_;
  std::string message_;
  bool len_is_read_;
};

}  // namespace tin

#endif  // SERVER_INSTRUCTIONS_SAY_H_
