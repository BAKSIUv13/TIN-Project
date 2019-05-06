// Copyright 2019 Piotrek

#ifndef SERVER_INSTRUCTIONS_CAPTURE_SESSION_H_
#define SERVER_INSTRUCTIONS_CAPTURE_SESSION_H_

#include <iostream>

#include "core/instr_struct.h"
#include "core/nquad.h"
#include "app/session.h"
#include "app/server.h"

namespace tin {
class CaptureSession : public InstrStruct {
 public:
  static constexpr int START = 2 * sizeof(NQuad);
  static constexpr int END = 4 * sizeof(NQuad);

  CaptureSession() {}
  virtual ~CaptureSession() {}
  static int Fn(Server *, int, SocketTCP4 *, SocketStuff *, World *);
  static void Construct(InstrStruct *);
  static void Destroy(InstrStruct *);
 private:
  constexpr const NQuad &first_() const {
    return reinterpret_cast<const NQuad *>(sess_id_)[0];
  }
  constexpr const NQuad &second_() const {
    return reinterpret_cast<const NQuad *>(sess_id_)[1];
  }
  constexpr SessionId GetSid_() const {
    return ((SessionId) 0 | first_())
    | (((SessionId) 0 | second_()) << (sizeof(NQuad) * 8));
  }


  alignas(NQuad) char sess_id_[2 * sizeof(NQuad)];
};


}  // namespace tin

#endif  // SERVER_INSTRUCTIONS_CAPTURE_SESSION_H_
