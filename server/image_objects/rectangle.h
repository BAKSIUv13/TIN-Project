// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_OBJECTS_RECTANGLE_H_
#define SERVER_IMAGE_OBJECTS_RECTANGLE_H_

#include <vector>

#include "image/basic_object.h"
#include "core/mquads.h"

namespace tin {

struct Rectangle : BasicObject {
  const char *GetType() {
    return "recatangle";
  }
  NQuad GetQuad() {
    return MQ::RECTANGLE;
  }
  Vec size;
};  // struct Rectangle

}  // namespace tin

#endif  // SERVER_IMAGE_OBJECTS_RECTANGLE_H_
