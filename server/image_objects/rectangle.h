// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_OBJECTS_RECTANGLE_H_
#define SERVER_IMAGE_OBJECTS_RECTANGLE_H_

#include <vector>
#include <memory>
#include <string>

#include "image/basic_object.h"
#include "image/utility.h"
#include "core/mquads.h"
#include "core/logger.h"

namespace tin {

struct Rectangle : BasicObject {
  virtual ~Rectangle() = default;
  const char *GetType() override {
    return "rectangle";
  }
  NQuad GetQuad() override {
    return MQ::SHAPE_RECTANGLE;
  }
  std::unique_ptr<BasicObject> CopyObject() override {
    return std::unique_ptr<BasicObject>(new Rectangle(*this));
  }
  void WriteToCppString(std::string *str) override {
    if (str == nullptr) return;
    GetQuad().AppendToCppString(str);
    NQuad fill = Utility::color_to_quad(fill_color);
    NQuad stroke = Utility::color_to_quad(stroke_color);
    fill.AppendToCppString(str);
    stroke.AppendToCppString(str);
    LogH << "Wysyłam kolor " << fill.GetHexStr() << stroke.GetHexStr() << '\n';
    NDouble(stroke_width).AppendToCppString(str);
    NDouble(transform[2][0]).AppendToCppString(str);
    NDouble(transform[2][1]).AppendToCppString(str);
    NDouble(dims.x).AppendToCppString(str);
    NDouble(dims.y).AppendToCppString(str);
  }
  Vec2 dims;

static void InitRectangle(Rectangle *rect, Color fill, Color stroke_c,
    Dist stroke_w,
    Vec2 offset, Vec2 dims) {
  if (rect == nullptr) return;
  rect->dims = dims;
  rect->fill_color = fill;
  rect->stroke_color = stroke_c;
  rect->stroke_width = stroke_w;
  rect->transform = Utility::translate(Transform(1.0), offset);
  LogH << "Zroiłem prostokąt " << Utility::color_to_hex_str(fill) <<
    Utility::color_to_hex_str(stroke_c) << '\n';
}

};  // struct Rectangle


}  // namespace tin

#endif  // SERVER_IMAGE_OBJECTS_RECTANGLE_H_
