// Copyright 2019 Piotrek

#ifndef SERVER_INSTRUCTIONS_LOG_IN_H_
#define SERVER_INSTRUCTIONS_LOG_IN_H_

#include <iostream>
#include <string>

#include "core/instr_struct.h"
#include "core/nquad.h"
#include "core/server.h"

namespace tin {
class LogIn : public InstrStruct {
 public:
  static constexpr int START = INSTR + NQS;

  LogIn() {}
  virtual ~LogIn() {}
  virtual int Fn(Server *, SocketStuff *, World *);

 private:
  constexpr int UnLen_() const {return START;}
  constexpr int Un_() const {return UnLen_() + NQS;}
  constexpr int PwLen_() const {return Un_() + un_len_;}
  constexpr int Pw_() const {return PwLen_() + NQS;}
  constexpr int End_() const {return Pw_() + pw_len_;}

  NQuad un_len_;
  NQuad pw_len_;
  std::string un_;
  std::string pw_;
};

}  // namespace tin

#endif  // SERVER_INSTRUCTIONS_LOG_IN_H_
