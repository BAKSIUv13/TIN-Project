// Copyright 2019 Piotrek

#ifndef SERVER_APP_SOCKET_STUFF_H_
#define SERVER_APP_SOCKET_STUFF_H_

#include <string>
#include <queue>

#include "network/nws.h"
#include "core/nquad.h"

namespace tin {

struct SocketStuff {
  static constexpr size_t BUF_SIZE = 256;
  static constexpr int MAX_SEGMENTS = 1024 * 1024;
  static constexpr bool END_ON_BAD_DATA = true;
  enum Error {
    OTHER = 1,
    CLOSE,
    NOT_OWO,
    AUTH_FAILED,
    SESS_CAPTURE_FAILED,
  };
  SocketStuff()
      : marked_to_delete(false), shall_read(false), shall_write(false),
        chars_loaded(0), which_segment(0) {
    buf[BUF_SIZE] = '\0';
  }
  char buf[BUF_SIZE + 1];
  bool marked_to_delete;
  bool shall_read;
  bool shall_write;
  alignas(NQuad) char first_quads[3 * sizeof(NQuad)];
  NQuad &magic() {
    return reinterpret_cast<NQuad *>(first_quads)[0];
  }
  NQuad &instr() {
    return reinterpret_cast<NQuad *>(first_quads)[1];
  }
  NQuad &instr2() {
    return reinterpret_cast<NQuad *>(first_quads)[2];
  }
  int chars_loaded;  // of actual message
  int which_segment;  // same
  std::queue<Error> errors;
};

}  // namespace tin

#endif  // SERVER_APP_SOCKET_STUFF_H_
