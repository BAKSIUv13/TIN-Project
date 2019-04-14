// Copyright 0 0

#ifndef SERVER_CORE_NQUAD_H_
#define SERVER_CORE_NQUAD_H_

#include <cstdint>
#include <cstring>
#include <type_traits>

namespace tin {

struct NQuad {
  union {
    alignas(int32_t) char c[4];
    uint32_t raw_uint;
  };
  explicit NQuad(int32_t);
  explicit NQuad(uint32_t);

  int32_t Int() const;
  uint32_t Uint() const;
  explicit operator int32_t() const;
  explicit operator uint32_t() const;
};

}  // namespace tin

#endif  // SERVER_CORE_NQUAD_H_
