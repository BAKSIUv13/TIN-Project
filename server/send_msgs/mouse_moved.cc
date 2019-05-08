// Copyright 2019 Piotrek

#include <iostream>
#include <utility>

#include "send_msgs/mouse_moved.h"
#include "core/mquads.h"

namespace tin {

int MouseMoved::AddToBuf(WriteBuf *buf) {
  int NQS = sizeof(NQuad);
  std::string str;
  str.append(MQ::OWO.CStr(), NQS);
  str.append(MQ::SERVER_MOUSE.CStr(), NQS);
  const char *name = GetUsername();
  str.append(name, 16);
  str.append(NQuad(x()).CStr(), NQS);
  str.append(NQuad(y()).CStr(), NQS);
  return buf->Add(str);
}
}  // namespace tin
