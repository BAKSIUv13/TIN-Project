// Copyright 2019 Piotrek

#ifndef SERVER_CORE_SOCK_STREAM_STATE_H_
#define SERVER_CORE_SOCK_STREAM_STATE_H_

#include <string>
#include <queue>

#include "network/nws.h"
#include "core/nquad.h"

namespace tin {

struct SockStreamState {
  static constexpr size_t BUF_SIZE = 256;
  static constexpr size_t MAX_SEGMENTS = 1024 * 1024;
  static constexpr bool END_ON_BAD_DATA = true;
  enum Error {
    OTHER = 1,
    NOT_OWO,
    AUTH_FAILED,
    SESS_CAPTURE_FAILED,
  };
  SockStreamState()
      : chars_loaded(0), which_segment(0) {
    buf[BUF_SIZE] = '\0';
  }
  union {
    char buf[BUF_SIZE + 1];
    NQuad qbuf[BUF_SIZE / sizeof(NQuad)];
  };
  NQuad instr_quad;
  size_t chars_loaded;
  size_t which_segment;
  std::queue<Error> errors;
};

}  // namespace tin

#endif  // SERVER_CORE_SOCK_STREAM_STATE_H_
