// Copyright 2019 Piotrek

#ifndef SERVER_IMAGE_OBJECTS_LINE_PATH_H_
#define SERVER_IMAGE_OBJECTS_LINE_PATH_H_

#include <vector>
#include <memory>
#include <string>

#include "image/basic_object.h"
#include "core/mquads.h"

namespace tin {

struct LinePath : BasicObject {
  virtual ~LinePath() = default;
  const char *GetType() override {
    return "line_path";
  }
  NQuad GetQuad() override {
    return MQ::SHAPE_POLYGONAL_CHAIN;
  }
  std::unique_ptr<BasicObject> CopyObject() override {
    return std::unique_ptr<BasicObject>(new LinePath(*this));
  }
  void WriteToCppString(std::string *str) override {
    if (str == nullptr) return;
    GetQuad().AppendToCppString(str);
  }
  std::vector<Vec2> points;
};  // struct LinePath

}  // namespace tin

#endif  // SERVER_IMAGE_OBJECTS_LINE_PATH_H_
