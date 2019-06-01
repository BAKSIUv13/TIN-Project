// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <memory>

#include "instructions/create_shape.h"

#include "send_msgs/user_msg.h"

#include "send_msgs/sig.h"

namespace tin {
int CreateShape::Fn(Server *server, SocketStuff *stuff, World *world,
    MsgPushFn push_fn) {
  LogH << "xdxdxd\n";
  int pom;
  switch (GetStage_(stuff->CmProcessed())) {
    case Stage::READ_TYPE:
      pom = stuff->ReadQuad(START, &shape_type_);
      if (pom != 0) return pom;
    case Stage::DEAL_WITH_SHAPE:
      switch (shape_type_) {
        case MQ::SHAPE_ELLIPSE:
          break;
        case MQ::SHAPE_POLYGONAL_CHAIN:
          break;
        case MQ::SHAPE_RECTANGLE:
          break;
        default:
          LogM << "bad shape\n";
          return -1;
      }
      break;
    default:
      return -1;
  }
  return 0;
}

}  // namespace tin
