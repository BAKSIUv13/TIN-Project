// Copyright 2019 Piotrek

#ifndef SERVER_INSTRUCTIONS_SAY_H_
#define SERVER_INSTRUCTIONS_SAY_H_

#include <iostream>
#include <string>

#include "core/instr_struct.h"
#include "core/nquad.h"
#include "app/session.h"
#include "app/server.h"

namespace tin {
class Say : public InstrStruct {
 public:
  static constexpr int START = 2 * sizeof(NQuad);
  static constexpr int LEN_END = 3 * sizeof(NQuad);

  Say() {}
  virtual ~Say() {}
  static int Fn(Server *, int, SocketTCP4 *, SocketStuff *, World *);
  static void Construct(InstrStruct *);
  static void Destroy(InstrStruct *);
 private:
  constexpr int32_t Len_() const {
    return reinterpret_cast<const NQuad *>(len_buf_)[0].Int();
  }

  alignas(NQuad) char len_buf_[sizeof(NQuad)];
  Username un_;
  std::string message_;
};


}  // namespace tin

#endif  // SERVER_INSTRUCTIONS_SAY_H_
