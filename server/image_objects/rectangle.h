// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_OBJECTS_RECTANGLE_H_
#define SERVER_IMAGE_OBJECTS_RECTANGLE_H_

#include <vector>
#include <memory>

#include "image/basic_object.h"
#include "core/mquads.h"

namespace tin {

struct Rectangle : BasicObject {
  const char *GetType() override {
    return "rectangle";
  }
  NQuad GetQuad() override {
    return MQ::SHAPE_RECTANGLE;
  }
  std::unique_ptr<BasicObject> CopyObject() override {
    return std::unique_ptr<BasicObject>(new Rectangle(*this));
  }
  Vec2 dims;
};  // struct Rectangle

}  // namespace tin

#endif  // SERVER_IMAGE_OBJECTS_RECTANGLE_H_
