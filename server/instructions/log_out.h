// Copyright 2019 Piotrek

#ifndef SERVER_INSTRUCTIONS_LOG_OUT_H_
#define SERVER_INSTRUCTIONS_LOG_OUT_H_

#include <iostream>
#include <string>

#include "core/instr_struct.h"
#include "core/nquad.h"
#include "app/server.h"

namespace tin {
class LogOut : public InstrStruct {
 public:
  static constexpr int START = INSTR + NQS;
  static constexpr int END = START + NQS;

  LogOut() {}
  virtual ~LogOut() {}
  static int Fn(Server *, SocketStuff *, World *, MsgPushFn);
  static void Construct(InstrStruct *);
  static void Destroy(InstrStruct *);
};

}  // namespace tin

#endif  // SERVER_INSTRUCTIONS_LOG_OUT_H_
