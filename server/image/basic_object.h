// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_BASIC_OBJECT_H_
#define SERVER_IMAGE_BASIC_OBJECT_H_

#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>

#include "core/nquad.h"

namespace tin {
struct BasicObject {
  using Dist = double;
  using Vec = glm::vec<2, Dist>;
  using Transform = glm::mat<3, 3, Dist>;

  Transform transform;
  virtual const char *GetType() = 0;
  virtual NQuad GetQuad() = 0;
};
}  // namespace tin

#endif  // SERVER_IMAGE_BASIC_OBJECT_H_
