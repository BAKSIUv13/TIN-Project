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
  static constexpr int START = 2 * sizeof(NQuad);
  static constexpr int END = 4 * sizeof(NQuad);

  MoveMouse() {}
  virtual ~MoveMouse() {}
  static int Fn(Server *, int, SocketStuff *, World *, MsgPushFn);
  static void Construct(InstrStruct *);
  static void Destroy(InstrStruct *);

 private:
  constexpr const NQuad &first_() const {
    return reinterpret_cast<const NQuad *>(coords_)[0];
  }
  constexpr const NQuad &second_() const {
    return reinterpret_cast<const NQuad *>(coords_)[1];
  }
  constexpr int32_t x() const {
    return first_().Int();
  }
  constexpr int32_t y() const {
    return second_().Int();
  }

  Username un_;
  alignas(NQuad) char coords_[2 * sizeof(NQuad)];
};


}  // namespace tin

#endif  // SERVER_INSTRUCTIONS_MOVE_MOUSE_H_
