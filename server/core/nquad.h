// Copyright 2019 Piotrek

#ifndef SERVER_CORE_NQUAD_H_
#define SERVER_CORE_NQUAD_H_

#include <endian.h>

#include <cstdint>
#include <cstring>
#include <type_traits>
#include <array>
#include <string>

namespace tin {

struct NQuad {
  uint32_t raw_uint;
  constexpr NQuad()
    : raw_uint(0) {
  }
  constexpr explicit NQuad(const char tab[4])
    : raw_uint {
      #if __BYTE_ORDER == __LITTLE_ENDIAN
        (((((
          ((uint32_t)0 | tab[3]) << 8)
            | tab[2]) << 8) | tab[1]) << 8) | tab[0]
      #elif __BYTE_ORDER == __BIG_ENDIAN
        (((((
          ((uint32_t)0 | tab[0]) << 8)
            | tab[1]) << 8) | tab[2]) << 8) | tab[3]
      #else
        #error :<
      #endif
    } {}
  constexpr explicit NQuad(uint32_t x)
    : raw_uint {
      #if __BYTE_ORDER == __LITTLE_ENDIAN
        (uint32_t)0
        | ((x & 0x000000ff) << 24)
        | ((x & 0x0000ff00) << 8)
        | ((x & 0x00ff0000) >> 8)
        | ((x & 0xff000000) >> 24)
      #elif __BYTE_ORDER == __BIG_ENDIAN
        x
      #else
        #error :<
      #endif
    } {}
  constexpr explicit NQuad(int32_t x)
    : NQuad(static_cast<uint32_t>(x)) {}

  constexpr bool operator==(const NQuad &o) {return raw_uint == o.raw_uint;}
  constexpr bool operator!=(const NQuad &o) {return raw_uint != o.raw_uint;}

  constexpr uint32_t Uint() const {
    return
      #if __BYTE_ORDER == __LITTLE_ENDIAN
        (uint32_t)0
        | ((raw_uint & 0x000000ff) << 24)
        | ((raw_uint & 0x0000ff00) << 8)
        | ((raw_uint & 0x00ff0000) >> 8)
        | ((raw_uint & 0xff000000) >> 24)
      #elif __BYTE_ORDER == __BIG_ENDIAN
        raw_uint
      #else
        #error :<
      #endif
    | 0;
  }
  constexpr int32_t Int() const {
    return static_cast<int32_t>(Uint());
  }
  constexpr operator uint32_t() const {return Uint();}
  constexpr explicit operator int32_t() const {return Int();}

  std::array<char, 4> CharArray() const {
    return *reinterpret_cast<const std::array<char, 4> *>(&raw_uint);
  }

  void AppendToCppString(std::string *s) const {
    s->append(this->CStr(), sizeof(*this));
  }

  operator std::string() const {
    std::string s;
    this->AppendToCppString(&s);
    return s;
  }

  constexpr uint8_t operator[](size_t n) const {
    return n < 4 ? reinterpret_cast<const uint8_t *>(this)[n] : '\0';
  }

  // Ostrożnie z tym, nie ma zera na końcu.
  const char *CStr() const {return reinterpret_cast<const char *>(&raw_uint);}
};

}  // namespace tin

#endif  // SERVER_CORE_NQUAD_H_
