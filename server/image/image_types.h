// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_IMAGE_TYPES_H_
#define SERVER_IMAGE_IMAGE_TYPES_H_

#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>

namespace tin {

inline namespace ImageTypes {
  using ObjectId = int32_t;

  using Dist = double;
  using Vec2 = glm::vec<2, Dist>;
  using Vec3 = glm::vec<3, Dist>;
  using Transform = glm::mat<3, 3, Dist>;
  using Color = glm::vec<4, uint8_t>;

  enum ImageErr : ObjectId {
    IMG_ERR_MAX_COUNT = -1,
    IMG_ERR_MAX_ID = -2,
    IMG_ERR_OTHER = -3,
  };
}

}  // namespace tin

#endif  // SERVER_IMAGE_IMAGE_TYPES_H_
