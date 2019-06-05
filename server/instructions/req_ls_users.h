// Copyright 2019 Piotrek

#ifndef SERVER_INSTRUCTIONS_REQ_LS_USERS_H_
#define SERVER_INSTRUCTIONS_REQ_LS_USERS_H_

#include <iostream>
#include <string>
#include <memory>

#include "core/instr_struct.h"
#include "core/nquad.h"
#include "core/server.h"

namespace tin {
class ReqLsUsers : public InstrStruct {
 public:
  static constexpr int START = INSTR + NQS;
  static constexpr int END = START + NQS;

  ReqLsUsers() {}
  virtual ~ReqLsUsers() {}
  virtual int Fn(Server *, SocketStuff *, World *);
};

}  // namespace tin

#endif  // SERVER_INSTRUCTIONS_REQ_LS_USERS_H_
