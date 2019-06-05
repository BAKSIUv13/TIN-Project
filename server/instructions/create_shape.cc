// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <memory>

#include "instructions/create_shape.h"

#include "send_msgs/user_msg.h"
#include "send_msgs/new_obj.h"

#include "send_msgs/sig.h"

#include "image_objects/rectangle.h"
#include "image_objects/ellipse.h"
#include "image_objects/line_path.h"
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
        case MQ::SHAPE_POLYGONAL_CHAIN:
          pom = stuff->ReadQuad(LINE_STROKE_COLOR_OFFSET, &colors_[1]);
          if (pom) return pom;
          pom = stuff->ReadDouble(LINE_STROKE_WIDTH_OFFSET, &stroke_width_);
          if (pom) return pom;
          pom = stuff->ReadQuad(LINE_POINT_COUNT_OFFSET, &point_count_);
          if (pom) return pom;
          if (point_count_ > LinePath::MAX_POINT_COUNT) {
            LogH << "Otrzymano za długą łamaną\n";
            server->PushMsg<Sig>(stuff->GetId(), MQ::ERR_OTHER, true);
            return -1;
          }
          for (; points_readd_ < point_count_; ++points_readd_) {
            pom = stuff->ReadDouble
              (LINE_FIRST_OFFSET + points_readd_ * LINE_SHIFT, &x_);
            if (pom) return pom;
            pom = stuff->ReadDouble
              (LINE_FIRST_OFFSET + points_readd_ * LINE_SHIFT + sizeof(NDouble),
              &y_);
            if (pom) return pom;
            points_.push_back(Vec2(x_, y_));
          }
          break;
        case MQ::SHAPE_RECTANGLE:
        case MQ::SHAPE_ELLIPSE:
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
            if (shape_type_ == MQ::SHAPE_RECTANGLE) {
              auto obj = world->AddObject<Rectangle>(un_);
              if (obj.first <= 0) {
                server->PushMsg<Sig>(stuff->GetId(), MQ::ERR_OTHER, false,
                  "could not add object");
                return 0;
              }
              InitRectangle(
                obj.second,
                Utility::quad_to_color(colors_[0]),
                Utility::quad_to_color(colors_[1]),
                stroke_width_,
                Vec2(x_, y_),
                Vec2(width_, height_));
              server->PushMsg<NewObj>
                (un_, obj.first, obj.second->GetCppString());
            } else {  // MQ::SHAPE_ELLIPSE
              auto obj = world->AddObject<Ellipse>(un_);
              if (obj.first <= 0) {
                server->PushMsg<Sig>(stuff->GetId(), MQ::ERR_OTHER, false,
                  "could not add object");
                return 0;
              }
              InitEllipse(
                obj.second,
                Utility::quad_to_color(colors_[0]),
                Utility::quad_to_color(colors_[1]),
                stroke_width_,
                Vec2(x_, y_),
                Vec2(width_, height_));
              server->PushMsg<NewObj>
                (un_, obj.first, obj.second->GetCppString());
            }
          } else {
            LogM << "Niezalog' gn' próbowało rysować prostokąt/elipsę\n";
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
