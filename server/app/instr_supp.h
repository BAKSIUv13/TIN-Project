// Copyright 2019 Piotrek

#ifndef SERVER_APP_INSTR_SUPP_H_
#define SERVER_APP_INSTR_SUPP_H_

#include <functional>

#include "core/instr_fn.h"
#include "core/construct_fn.h"
#include "core/destroy_fn.h"

namespace tin {
class Server;
class SocketTCP4;
class SocketStuff;
class InstrSupp {
 public:
  static constexpr struct EXPAND_t {} EXPAND {};
  constexpr InstrSupp()
    : expands_(false), fn_(nullptr), mem_size_(0), constructor_fn_(nullptr),
      destructor_fn_(nullptr) {}
  constexpr explicit InstrSupp(InstrFn f, size_t mem_size, ConstructFn c,
      DestroyFn d)
    : expands_(false), fn_(f), mem_size_(mem_size), constructor_fn_(c),
      destructor_fn_(d) {}
  constexpr explicit InstrSupp(EXPAND_t)
    : expands_(true), fn_(nullptr), mem_size_(0), constructor_fn_(nullptr),
      destructor_fn_(nullptr) {}

  constexpr InstrFn GetFn() const {return fn_;}
  constexpr bool Expands() const {return expands_;}
  constexpr operator bool() const {return expands_ || fn_;}
  constexpr bool Blank() const {return !operator bool();}
  constexpr size_t GetSize() const {return mem_size_;}
  constexpr ConstructFn GetConstructor() const {return constructor_fn_;}
  constexpr DestroyFn GetDestructor() const {return destructor_fn_;}

 private:
  bool expands_;
  InstrFn fn_;
  size_t mem_size_;
  ConstructFn constructor_fn_;
  DestroyFn destructor_fn_;
};
}  // namespace tin


#endif  // SERVER_APP_INSTR_SUPP_H_
