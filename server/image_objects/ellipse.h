// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_OBJECTS_ELLIPSE_H_
#define SERVER_IMAGE_OBJECTS_ELLIPSE_H_

#include <vector>
#include <memory>
#include <string>

#include "image/basic_object.h"
#include "core/mquads.h"

namespace tin {

struct Ellipse : BasicObject {
  virtual ~Ellipse() = default;
  const char *GetType() const override {
    return "ellipse";
  }
  NQuad GetQuad() const override {
    return MQ::SHAPE_ELLIPSE;
  }
  std::unique_ptr<BasicObject> CopyObject() const override {
    return std::unique_ptr<BasicObject>(new Ellipse(*this));
  }
  void WriteToCppString(std::string *str) const override {
    if (str == nullptr) return;
    GetQuad().AppendToCppString(str);
  }
  // Vec centrum;  // <-- nie potrzebne, jak mamy transform
  Vec2 radii;
};  // struct Ellipse

}  // namespace tin

#endif  // SERVER_IMAGE_OBJECTS_ELLIPSE_H_
