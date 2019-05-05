// Copyright 2019 Piotrek

#ifndef SERVER_CORE_INSTR_ID_H_
#define SERVER_CORE_INSTR_ID_H_

#include "core/nquad.h"

namespace tin {
class InstrId {
 public:
  // Default constructor which makes useless blank id.
  constexpr InstrId() {}

  // Constructor of single instruction ID.
  constexpr explicit InstrId(NQuad);

  // Double instruction ID.
  constexpr explicit InstrId(NQuad, NQuad);

  constexpr bool operator<(const InstrId &other) const {
    return first_ < other.first_
      || (first_ == other.first_ && second_ < other.second_);
  }

  NQuad GetFirst() {return first_;}
  NQuad Get() {return first_;}
  NQuad GetSecond() {return second_;}
 private:
  NQuad first_;
  NQuad second_;
};

constexpr InstrId::InstrId(NQuad f)
    : first_(f) {}

constexpr InstrId::InstrId(NQuad f, NQuad s)
    : first_(f), second_(s) {}

}  // namespace tin

#endif  // SERVER_CORE_INSTR_ID_H_
