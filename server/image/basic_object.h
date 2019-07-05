// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_BASIC_OBJECT_H_
#define SERVER_IMAGE_BASIC_OBJECT_H_

#include <memory>
#include <string>

#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>

#include "image/image_types.h"
#include "core/nquad.h"

namespace tin {
struct BasicObject {
  BasicObject()
    : fill_color{0.0, 0.0, 0.0, 1.0}, stroke_color{0.0, 0.0, 0.0, 1.0},
      stroke_width(0.0) {}
  virtual ~BasicObject() = default;

  Transform transform;

  Color fill_color;
  Color stroke_color;

  Dist stroke_width;

  virtual const char *GetType() const = 0;
  virtual NQuad GetQuad() const = 0;
  virtual std::unique_ptr<BasicObject> CopyObject() const = 0;
  virtual void WriteToCppString(std::string *) const = 0;
  virtual std::string GetCppString() const final {
    std::string str;
    WriteToCppString(&str);
    return str;
  }
};
}  // namespace tin

#endif  // SERVER_IMAGE_BASIC_OBJECT_H_
