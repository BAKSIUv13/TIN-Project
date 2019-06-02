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
  static constexpr int RedOffset() {return SHAPE_THINGS + 0;}
  static constexpr int GreenOffset() {return SHAPE_THINGS + 1;}
  static constexpr int BlueOffset() {return SHAPE_THINGS + 2;}
  static constexpr int RectLeft() {return BlueOffset() + 1;}
  static constexpr int RectTop() {return RectLeft() + sizeof(NDouble);}
  static constexpr int RectRight() {return RectTop() + sizeof(NDouble);}
  static constexpr int RectBottom() {return RectRight() + sizeof(NDouble);}

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
  uint8_t r_;
  uint8_t g_;
  uint8_t b_;

  NDouble rl_, rt_, rr_, rb_;
};

}  // namespace tin

#endif  // SERVER_INSTRUCTIONS_CREATE_SHAPE_H_
