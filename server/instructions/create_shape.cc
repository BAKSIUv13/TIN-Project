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
          pom = stuff->ReadByte(RedOffset(), &r_);
          if (pom) return pom;
          pom = stuff->ReadByte(GreenOffset(), &g_);
          if (pom) return pom;
          pom = stuff->ReadByte(BlueOffset(), &b_);
          if (pom) return pom;
          pom = stuff->ReadDouble(RectLeft(), &rl_);
          if (pom) return pom;
          pom = stuff->ReadDouble(RectTop(), &rt_);
          if (pom) return pom;
          pom = stuff->ReadDouble(RectRight(), &rr_);
          if (pom) return pom;
          pom = stuff->ReadDouble(RectBottom(), &rb_);
          if (pom) return pom;
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
