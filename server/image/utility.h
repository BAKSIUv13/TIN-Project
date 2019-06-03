// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_UTILITY_H_
#define SERVER_IMAGE_UTILITY_H_

#include <limits>

#include <glm/gtc/matrix_transform.hpp>

#include "image/image_types.h"
#include "image_objects/rectangle.h"

namespace tin {

namespace Utility {
  // Thiis is almost copy of function from glm, but it is on 2-dimensional
  // vector instead of 3.
  template<typename T, glm::qualifier Q>
  GLM_FUNC_QUALIFIER glm::mat<3, 3, T, Q> translate
      (glm::mat<3, 3, T, Q> const& m,
      glm::vec<2, T, Q> const& v) {
    glm::mat<3, 3, T, Q> res(m);
    res[2] = m[0] * v[0] + m[1] * v[1] + m[2];
    return res;
  }

  inline Color quad_to_color(NQuad quad) {
    return Color{quad[0], quad[1], quad[2], quad[3]};
  }

  inline NQuad color_to_quad(Color color) {
    char x[4]{
      static_cast<char>(color.x),
      static_cast<char>(color.y),
      static_cast<char>(color.z),
      static_cast<char>(color.t),
    };
    return NQuad{x};
  }

  inline void InitRectangle(Rectangle *rect, Color fill, Color stroke_c,
      Dist stroke_w,
      Vec2 offset, Vec2 dims) {
    if (rect == nullptr) return;
    rect->dims = dims;
    rect->fill_color = fill;
    rect->stroke_color = stroke_c;
    rect->stroke_width = stroke_w;
    rect->transform = translate(Transform(1.0), offset);
  }
}  // namespace Utility

}  // namespace tin

#endif  // SERVER_IMAGE_UTILITY_H_
