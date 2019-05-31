// Copyright 2019 Piotrek

#ifndef SERVER_INSTRUCTIONS_SAY_H_
#define SERVER_INSTRUCTIONS_SAY_H_

#include <iostream>
#include <string>
#include <algorithm>

#include "core/instr_struct.h"
#include "core/nquad.h"
#include "app/session.h"
#include "app/server.h"

namespace tin {
class Say : public InstrStruct {
 public:
  static constexpr int START = INSTR + NQS;
  static constexpr int LEN = START;
  static constexpr int MSG = LEN + NQS;
  static constexpr int32_t LEN_CUT {2048};

  Say() : len_is_read_(false) {}
  virtual ~Say() {}
  static int Fn(Server *, SocketStuff *, World *, MsgPushFn);
  static void Construct(InstrStruct *);
  static void Destroy(InstrStruct *);
 private:
  constexpr int32_t Len_() const {
    return std::min(len_.Int(), LEN_CUT);
  }

  NQuad len_;
  Username un_;
  std::string message_;
  bool len_is_read_;
};

}  // namespace tin

#endif  // SERVER_INSTRUCTIONS_SAY_H_
