// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_OBJECTS_ELLIPSE_H_
#define SERVER_IMAGE_OBJECTS_ELLIPSE_H_

#include <vector>

#include "image/basic_object.h"
#include "core/mquads.h"

namespace tin {

struct Ellipse : BasicObject {
  const char *GetType() {
    return "ellipse";
  }
  NQuad GetQuad() {
    return MQ::ELLIPSE;
  }
  // Vec centrum;  // <-- nie potrzebne, jak mamy transform
  Vec radii;
};  // struct Ellipse

}  // namespace tin

#endif  // SERVER_IMAGE_OBJECTS_ELLIPSE_H_
