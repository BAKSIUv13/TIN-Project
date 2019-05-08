// Copyright 2019 Piotrek

#include <iostream>
#include <utility>

#include "send_msgs/user_msg.h"
#include "core/mquads.h"

namespace tin {

int UserMsg::AddToBuf(WriteBuf *buf) {
  int NQS = sizeof(NQuad);
  std::string str;
  str.append(MQ::OWO.CStr(), NQS);
  str.append(MQ::SERVER_DELIVER_MSG.CStr(), NQS);
  const char *name = GetUsername();
  str.append(name, 16);
  uint32_t content_size = GetContent().size();
  str.append(NQuad(content_size).CStr(), NQS);
  str.append(GetContent());
  return buf->Add(str);
}
}  // namespace tin
