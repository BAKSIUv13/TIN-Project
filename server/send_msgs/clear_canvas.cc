// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <string>

#include "send_msgs/clear_canvas.h"
#include "core/mquads.h"

namespace tin {
std::string ClearCanvas::GetStr() const {
  std::string str;
  MQ::OWO.AppendToCppString(&str);
  MQ::SERV_CLEAR_CANVAS.AppendToCppString(&str);
  return str;
}
}  // namespace tin
