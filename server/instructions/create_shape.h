// Copyright 2019 TIN

#ifndef SERVER_INSTRUCTIONS_CREATE_SHAPE_H_
#define SERVER_INSTRUCTIONS_CREATE_SHAPE_H_

#include <iostream>
#include <string>
#include <algorithm>

#include "core/instr_struct.h"
#include "core/nquad.h"
#include "core/server.h"

namespace tin {
class CreateShape : public InstrStruct {
 public:
  static constexpr int START = INSTR + NQS;
  static constexpr int SHAPE_THINGS = START + NQS;

  CreateShape() : shape_type_() {}
  virtual ~CreateShape() {}
  virtual int Fn(Server *, SocketStuff *, World *, MsgPushFn);

 private:
  enum class Stage {
    BAD,
    READ_TYPE,
    DEAL_WITH_SHAPE
  };
  static constexpr Stage GetStage_(int offset) {
    return offset < START ? Stage::BAD :
      offset < SHAPE_THINGS ? Stage::READ_TYPE :
      Stage::DEAL_WITH_SHAPE;
  }
  // Username un_;
  NQuad shape_type_;
  // std::string message_;
  // bool len_is_read_;
};

}  // namespace tin

#endif  // SERVER_INSTRUCTIONS_CREATE_SHAPE_H_
