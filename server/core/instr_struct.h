// Copyright 2019 TIN

#ifndef SERVER_CORE_INSTR_STRUCT_H_
#define SERVER_CORE_INSTR_STRUCT_H_

#include <memory>
#include <utility>
#include <functional>

#include "core/nquad.h"
#include "core/instr_supp.h"

namespace tin {

class Server;
class SocketStuff;
class World;

class OutMessage;

// Class that shall be derived for wvery instruction that wants to have memory.
class InstrStruct {
 public:
  static constexpr int NQS = sizeof(NQuad);
  static constexpr int MAGIC = 0;
  static constexpr int INSTR = MAGIC + NQS;
  static constexpr int INSTR2 = INSTR + NQS;

  // using MsgPushFn = int(Server::*)(std::unique_ptr<OutMessage>);

  template <typename T>
  static InstrSupp Supp() {
    return InstrSupp{[](){
      return std::unique_ptr<InstrStruct>(new T());
    }};
  }

  InstrStruct() {}
  virtual ~InstrStruct() {}
  virtual int Fn(Server *, SocketStuff *, World *) = 0;
};

}  // namespace tin

#endif  // SERVER_CORE_INSTR_STRUCT_H_
