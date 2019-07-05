// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_UTILITY_H_
#define SERVER_IMAGE_UTILITY_H_

#include <limits>

#include <glm/gtc/matrix_transform.hpp>

#include "image/image_types.h"

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
      static_cast<char>(color.w),
    };
    return NQuad{x};
  }

  inline std::string color_to_hex_str(Color color) {
    std::string s;
    char c[9];
    snprintf(&c[0], 3, "%02x", color.x);
    s.append(&c[0], 2);
    snprintf(&c[2], 3, "%02x", color.y);
    s.append(&c[2], 2);
    snprintf(&c[4], 3, "%02x", color.z);
    s.append(&c[4], 2);
    snprintf(&c[6], 3, "%02x", color.w);
    s.append(&c[6], 2);
    return s;
  }

}  // namespace Utility

}  // namespace tin

#endif  // SERVER_IMAGE_UTILITY_H_
