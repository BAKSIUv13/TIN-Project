// Copyright 2019 Piotrek

#include <iostream>
#include <utility>

#include "send_msgs/new_obj.h"
#include "core/mquads.h"
#include "core/ndouble.h"
#include "image/utility.h"

namespace tin {

NewObj::~NewObj() {}

int NewObj::AddToBuf(WriteBuf *buf) {
  std::string str;
  MQ::OWO.AppendToCppString(&str);
  MQ::SERV_NEW_OBJECT.AppendToCppString(&str);
  NQuad(id_).AppendToCppString(&str);
  int un_len = username_.Len();
  NQuad(un_len).AppendToCppString(&str);
  str.append(username_.operator const char *(), un_len);
  shape_copy_->GetQuad().AppendToCppString(&str);
  switch (shape_copy_->GetQuad()) {
    case MQ::SHAPE_RECTANGLE:
        Utility::color_to_quad(shape_copy_->fill_color).AppendToCppString(&str);
        Utility::color_to_quad(shape_copy_->stroke_color).
          AppendToCppString(&str);
        NDouble(shape_copy_->stroke_width).AppendToCppString(&str);
        NDouble(shape_copy_->transform[2][0]).AppendToCppString(&str);
        NDouble(shape_copy_->transform[2][1]).AppendToCppString(&str);
        NDouble(dynamic_cast<Rectangle *>(&*shape_copy_)->dims.x).
          AppendToCppString(&str);
        NDouble(dynamic_cast<Rectangle *>(&*shape_copy_)->dims.y).
          AppendToCppString(&str);
      break;
    default:
      break;
  }
  return buf->Add(str);
}
}  // namespace tin
