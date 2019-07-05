// Copyright 2019 TIN

#ifndef SERVER_CORE_INSTR_SUPP_H_
#define SERVER_CORE_INSTR_SUPP_H_

#include <functional>
#include <memory>

#include "core/factory.h"

namespace tin {
class InstrSupp {
 public:
  static constexpr struct EXPAND_t {} EXPAND {};
  InstrSupp()
    : expands_(false), factory_(nullptr) {}
  explicit InstrSupp(Factory factory)
    : expands_(false), factory_(factory) {}
  explicit InstrSupp(EXPAND_t)
    : expands_(true), factory_(nullptr) {}

  constexpr bool Expands() const {return expands_;}
  constexpr operator bool() const {return expands_ || factory_;}
  constexpr bool Blank() const {return !operator bool();}

  std::unique_ptr<InstrStruct> Create() const {
    return factory_ ? factory_() : nullptr;
  }

 private:
  bool expands_;
  Factory factory_;
};
}  // namespace tin

#endif  // SERVER_CORE_INSTR_SUPP_H_
