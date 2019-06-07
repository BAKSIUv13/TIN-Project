// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <string>

#include "send_msgs/clear_canvas.h"
#include "core/mquads.h"

namespace tin {
int ClearCanvas::AddToBuf(WriteBuf *buf) const {
  std::string str;
  MQ::OWO.AppendToCppString(&str);
  MQ::SERV_CLEAR_CANVAS.AppendToCppString(&str);
  return buf->Add(str);
}
}  // namespace tin
