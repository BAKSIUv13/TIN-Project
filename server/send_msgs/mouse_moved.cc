// Copyright 2019 Piotrek

#include <iostream>
#include <utility>

#include "send_msgs/mouse_moved.h"
#include "core/mquads.h"
#include "core/ndouble.h"

namespace tin {

int MouseMoved::AddToBuf(WriteBuf *buf) {
  std::string str;
  MQ::OWO.AppendToCppString(&str);
  MQ::SERV_MOUSE.AppendToCppString(&str);
  NDouble(x()).AppendToCppString(&str);
  NDouble(y()).AppendToCppString(&str);
  const char *name = GetUsername();
  NQuad(static_cast<uint32_t>(strlen(name))).AppendToCppString(&str);
  str.append(name);
  return buf->Add(str);
}
}  // namespace tin
