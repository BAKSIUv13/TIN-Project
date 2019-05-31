// Copyright 2019 Piotrek

#ifndef SERVER_INSTRUCTIONS_MOVE_MOUSE_H_
#define SERVER_INSTRUCTIONS_MOVE_MOUSE_H_

#include <iostream>

#include "core/instr_struct.h"
#include "core/nquad.h"
#include "app/session.h"
#include "app/server.h"

namespace tin {
class MoveMouse : public InstrStruct {
 public:
  static constexpr int START = INSTR + NQS;
  static constexpr int END = 4 * sizeof(NQuad);

  MoveMouse() {}
  virtual ~MoveMouse() {}
  static int Fn(Server *, SocketStuff *, World *, MsgPushFn);
  static void Construct(InstrStruct *);
  static void Destroy(InstrStruct *);

 private:
  Username un_;
  double x;
  double y;
};

}  // namespace tin

#endif  // SERVER_INSTRUCTIONS_MOVE_MOUSE_H_
