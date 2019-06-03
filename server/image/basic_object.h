// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_BASIC_OBJECT_H_
#define SERVER_IMAGE_BASIC_OBJECT_H_

#include <memory>

#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>

#include "image/image_types.h"
#include "core/nquad.h"

namespace tin {
struct BasicObject {
  BasicObject()
    : fill_color{0.0, 0.0, 0.0, 1.0}, stroke_color{0.0, 0.0, 0.0, 1.0},
      stroke_width(0.0) {}
  BasicObject(const BasicObject &) = default;
  virtual ~BasicObject() = default;

  Transform transform;

  Color fill_color;
  Color stroke_color;

  Dist stroke_width;

  virtual const char *GetType() = 0;
  virtual NQuad GetQuad() = 0;
  virtual std::unique_ptr<BasicObject> CopyObject() = 0;
};
}  // namespace tin

#endif  // SERVER_IMAGE_BASIC_OBJECT_H_
