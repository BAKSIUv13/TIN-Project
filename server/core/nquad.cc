// Copyright 0 0

#include <arpa/inet.h>
#include <cstring>

#include "core/nquad.h"

namespace tin {

static inline uint32_t match_endianness(const NQuad *nq) {
  uint32_t number;
  memcpy(&number, nq->c, sizeof(uint32_t));
  return ntohl(number);
}

int32_t NQuad::Int() const {
  return static_cast<int32_t>(match_endianness(this));
}

uint32_t NQuad::Uint() const {
  return match_endianness(this);
}

NQuad::operator int32_t() const {
  return static_cast<int32_t>(match_endianness(this));
}

NQuad::operator uint32_t() const {
  return match_endianness(this);
}

NQuad::NQuad(int32_t n) {
  uint32_t un = htonl(n);
  memcpy(c, &un, sizeof(uint32_t));
}

NQuad::NQuad(uint32_t n) {
  n = htonl(n);
  memcpy(c, &n, sizeof(uint32_t));
}

}  // namespace tin
