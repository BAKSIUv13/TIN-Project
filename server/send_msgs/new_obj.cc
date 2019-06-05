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
  str.append(username_, un_len);
  str.append(shape_code_);
  return buf->Add(str);
}
}  // namespace tin
