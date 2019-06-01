// Copyright 2019 Piotrek

#include <iostream>
#include <utility>

#include "send_msgs/mouse_moved.h"
#include "core/mquads.h"
#include "core/ndouble.h"

namespace tin {

int MouseMoved::AddToBuf(WriteBuf *buf) {
  std::string str;
  MQ::OWO.AppendToCpp11String(&str);
  MQ::SERV_MOUSE.AppendToCpp11String(&str);
  NDouble(x()).AppendToCpp11String(&str);
  NDouble(y()).AppendToCpp11String(&str);
  const char *name = GetUsername();
  NQuad(static_cast<uint32_t>(strlen(name))).AppendToCpp11String(&str);
  str.append(name);
  return buf->Add(str);
}
}  // namespace tin
