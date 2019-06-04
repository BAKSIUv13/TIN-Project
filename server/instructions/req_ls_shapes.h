// Copyright 2019 Piotrek

#ifndef SERVER_INSTRUCTIONS_REQ_LS_SHAPES_H_
#define SERVER_INSTRUCTIONS_REQ_LS_SHAPES_H_

#include <iostream>
#include <string>
#include <memory>

#include "core/instr_struct.h"
#include "core/nquad.h"
#include "core/server.h"

namespace tin {
class ReqLsShapes : public InstrStruct {
 public:
  static constexpr int START = INSTR + NQS;
  static constexpr int END = START + NQS;

  ReqLsShapes() {}
  virtual ~ReqLsShapes() {}
  virtual int Fn(Server *, SocketStuff *, World *);
};

}  // namespace tin

#endif  // SERVER_INSTRUCTIONS_REQ_LS_SHAPES_H_
