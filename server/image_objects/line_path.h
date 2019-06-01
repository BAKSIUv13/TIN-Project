// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_OBJECTS_LINE_PATH_H_
#define SERVER_IMAGE_OBJECTS_LINE_PATH_H_

#include <vector>

#include "image/basic_object.h"
#include "core/mquads.h"

namespace tin {

struct LinePath : BasicObject {
  const char *GetType() {
    return "line_path";
  }
  NQuad GetQuad() {
    return MQ::POLYGONAL_CHAIN;
  }
  std::vector<Vec> points;
};  // struct LinePath

}  // namespace tin

#endif  // SERVER_IMAGE_OBJECTS_LINE_PATH_H_
