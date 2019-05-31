// Copyright 2019 Piotrek

#ifndef SERVER_INSTRUCTIONS_LOG_IN_H_
#define SERVER_INSTRUCTIONS_LOG_IN_H_

#include <iostream>
#include <string>

#include "core/instr_struct.h"
#include "core/nquad.h"
#include "app/server.h"

namespace tin {
class LogIn : public InstrStruct {
 public:
  // static constexpr InstrSupp supp(&LogIn::Fn, sizeof(LogIn))

  static constexpr int START = INSTR + NQS;

  LogIn() {}
  virtual ~LogIn() {}
  static int Fn(Server *, SocketStuff *, World *, MsgPushFn);
  static void Construct(InstrStruct *);
  static void Destroy(InstrStruct *);

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
