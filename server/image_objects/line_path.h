// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_OBJECTS_LINE_PATH_H_
#define SERVER_IMAGE_OBJECTS_LINE_PATH_H_

#include <vector>
#include <memory>
#include <string>
#include <utility>

#include "image/basic_object.h"
#include "image/utility.h"
#include "core/nquad.h"
#include "core/ndouble.h"
#include "core/mquads.h"

namespace tin {

struct LinePath : BasicObject {
  static constexpr size_t MAX_POINT_COUNT = 12000;

  virtual ~LinePath() = default;
  const char *GetType() const override {
    return "line_path";
  }
  NQuad GetQuad() const override {
    return MQ::SHAPE_POLYGONAL_CHAIN;
  }
  std::unique_ptr<BasicObject> CopyObject() const override {
    return std::unique_ptr<BasicObject>(new LinePath(*this));
  }
  void WriteToCppString(std::string *str) const override {
    if (str == nullptr) return;
    GetQuad().AppendToCppString(str);
    NQuad stroke = Utility::color_to_quad(stroke_color);
    stroke.AppendToCppString(str);
    NDouble(stroke_width).AppendToCppString(str);
    NQuad(static_cast<uint32_t>(points_.size())).AppendToCppString(str);
    for (const auto &point : points_) {
      Vec2 v = transform * Vec3(point, 1.0);
      NDouble(v.x).AppendToCppString(str);
      NDouble(v.y).AppendToCppString(str);
    }
  }
  std::vector<Vec2> points_;
};  // struct LinePath

inline void InitLinePath(LinePath *l, Color stroke_c, Dist stroke_w,
    std::vector<Vec2> pts) {
  if (l == nullptr) return;
  l->transform = Transform();
  l->fill_color = Color();
  l->stroke_color = stroke_c;
  l->stroke_width = stroke_w;
  l->points_ = std::move(pts);
  // LogH << "Zroiłem prostokąt " << Utility::color_to_hex_str(fill) <<
  //   Utility::color_to_hex_str(stroke_c) << '\n';
}

}  // namespace tin

#endif  // SERVER_IMAGE_OBJECTS_LINE_PATH_H_
