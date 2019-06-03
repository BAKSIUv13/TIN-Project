// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <memory>

#include "instructions/create_shape.h"

#include "send_msgs/user_msg.h"
#include "send_msgs/new_obj.h"

#include "send_msgs/sig.h"

#include "image_objects/rectangle.h"
#include "image/utility.h"

#include "core/server.h"

namespace tin {
int CreateShape::Fn(Server *server, SocketStuff *stuff, World *world) {
  LogH << "xdxdxd\n";
  int pom;
  un_ = server->SockToUn(stuff->GetId());
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
          pom = stuff->ReadString(COLORS_OFFSET, sizeof(colors_), colors_);
          if (pom) return pom;
          pom = stuff->ReadDouble(STROKE_WIDTH_OFFSET, &stroke_width_);
          if (pom) return pom;
          pom = stuff->ReadDouble(X_OFFSET, &x_);
          if (pom) return pom;
          pom = stuff->ReadDouble(Y_OFFSET, &y_);
          if (pom) return pom;
          pom = stuff->ReadDouble(WIDTH_OFFSET, &width_);
          if (pom) return pom;
          pom = stuff->ReadDouble(HEIGHT_OFFSET, &height_);
          if (pom) return pom;
          if (un_) {
            auto obj = world->AddObject<Rectangle>(un_);
            Utility::InitRectangle(
              obj.second,
              Utility::quad_to_color(colors_[0]),
              Utility::quad_to_color(colors_[1]),
              stroke_width_,
              Vec2(x_, y_),
              Vec2(width_, height_));
            server->PushMsg<NewObj>(un_, obj.first, obj.second->CopyObject());
          } else {
            LogM << "Niezalog' gn' próbowało rysować prostokąt\n";
            server->PushMsg<Sig>(stuff->GetId(), MQ::ERR_NOT_LOGGED, false);
          }
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
