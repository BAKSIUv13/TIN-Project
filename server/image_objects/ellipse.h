// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_OBJECTS_ELLIPSE_H_
#define SERVER_IMAGE_OBJECTS_ELLIPSE_H_

#include <vector>
#include <memory>

#include "image/basic_object.h"
#include "core/mquads.h"

namespace tin {

struct Ellipse : BasicObject {
  const char *GetType() override {
    return "ellipse";
  }
  NQuad GetQuad() override {
    return MQ::SHAPE_ELLIPSE;
  }
  std::unique_ptr<BasicObject> CopyObject() override {
    return std::unique_ptr<BasicObject>(new Ellipse(*this));
  }
  // Vec centrum;  // <-- nie potrzebne, jak mamy transform
  Vec2 radii;
};  // struct Ellipse

}  // namespace tin

#endif  // SERVER_IMAGE_OBJECTS_ELLIPSE_H_
