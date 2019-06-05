// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_OBJECTS_ELLIPSE_H_
#define SERVER_IMAGE_OBJECTS_ELLIPSE_H_

#include <vector>
#include <memory>
#include <string>

#include "image/basic_object.h"
#include "image/utility.h"
#include "core/nquad.h"
#include "core/mquads.h"
#include "core/ndouble.h"

namespace tin {

struct Ellipse : BasicObject {
  virtual ~Ellipse() = default;
  const char *GetType() const override {
    return "ellipse";
  }
  NQuad GetQuad() const override {
    return MQ::SHAPE_ELLIPSE;
  }
  std::unique_ptr<BasicObject> CopyObject() const override {
    return std::unique_ptr<BasicObject>(new Ellipse(*this));
  }
  void WriteToCppString(std::string *str) const override {
    if (str == nullptr) return;
    GetQuad().AppendToCppString(str);
    NQuad fill = Utility::color_to_quad(fill_color);
    NQuad stroke = Utility::color_to_quad(stroke_color);
    fill.AppendToCppString(str);
    stroke.AppendToCppString(str);
    NDouble(stroke_width).AppendToCppString(str);
    double x, y, w, h;
    x = transform[2][0] - radii.x;
    y = transform[2][1] - radii.y;
    w = radii.x * 2.0;
    h = radii.y * 2.0;
    NDouble(x).AppendToCppString(str);
    NDouble(y).AppendToCppString(str);
    NDouble(w).AppendToCppString(str);
    NDouble(h).AppendToCppString(str);
  }
  // Vec centrum;  // <-- nie potrzebne, jak mamy transform
  Vec2 radii;
};  // struct Ellipse

inline void InitEllipse(Ellipse *oval, Color fill, Color stroke_c,
    Dist stroke_w,
    Vec2 corner1, Vec2 corner2_off) {
  if (oval == nullptr) return;
  oval->radii = corner2_off / 2.0;
  oval->fill_color = fill;
  oval->stroke_color = stroke_c;
  oval->stroke_width = stroke_w;
  oval->transform = Utility::translate(Transform(1.0), corner1 + oval->radii);
  // LogH << "Zroiłem prostokąt " << Utility::color_to_hex_str(fill) <<
  //   Utility::color_to_hex_str(stroke_c) << '\n';
}

}  // namespace tin

#endif  // SERVER_IMAGE_OBJECTS_ELLIPSE_H_
