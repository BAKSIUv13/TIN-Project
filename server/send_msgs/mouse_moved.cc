// Copyright 2019 Piotrek

#include <iostream>
#include <utility>

#include "send_msgs/mouse_moved.h"
#include "core/mquads.h"
#include "core/ndouble.h"

namespace tin {

std::string MouseMoved::GetStr() const {
  std::string str;
  MQ::OWO.AppendToCppString(&str);
  MQ::SERV_MOUSE.AppendToCppString(&str);
  NDouble(x()).AppendToCppString(&str);
  NDouble(y()).AppendToCppString(&str);
  NQuad(GetUsername().Len()).AppendToCppString(&str);
  str.append(GetUsername());
  return str;
}
}  // namespace tin
