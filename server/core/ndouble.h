// Copyright 2019 Piotrek

#ifndef SERVER_CORE_NDOUBLE_H_
#define SERVER_CORE_NDOUBLE_H_

#include <endian.h>

#include <cstdint>
#include <cstring>
#include <type_traits>
#include <array>
/*
static_assert(sizeof(uint64_t) == sizeof(double));
static_assert(__BYTE_ORDER == __LITTLE_ENDIAN);
static_assert(__FLOAT_WORD_ORDER == __LITTLE_ENDIAN);
#if !(__x86_64 || _M_X64 || __i386__ || __i486__ || __i586__ || __i686__ ||\
  _M_I86 || _M_IX86)
#error "bad arch xd"
#endif
*/
namespace tin {

struct NDouble {
  double x;
  constexpr NDouble()
    : x(0.0) {
  }
  constexpr bool operator==(const NDouble &o) {return x == o.x;}
  constexpr bool operator!=(const NDouble &o) {return x != o.x;}

  constexpr bool operator<(const NDouble &o) {return x < o.x;}
  constexpr bool operator<=(const NDouble &o) {return x <= o.x;}
  constexpr bool operator>(const NDouble &o) {return x > o.x;}
  constexpr bool operator>=(const NDouble &o) {return x >= o.x;}

  constexpr uint64_t Raw() const noexcept {
    return *reinterpret_cast<const uint64_t *>(&x);
  }

  constexpr double Double() const noexcept {
    return *reinterpret_cast<const double *>(&x);
  }

  constexpr operator double() const noexcept {
    return Double();
  }

};

}  // namespace tin

#endif  // SERVER_CORE_NDOUBLE_H_
