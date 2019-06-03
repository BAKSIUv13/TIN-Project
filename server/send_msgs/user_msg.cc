// Copyright 2019 Piotrek

#include <iostream>
#include <utility>

#include "send_msgs/user_msg.h"
#include "core/mquads.h"

namespace tin {

int UserMsg::AddToBuf(WriteBuf *buf) {
  std::string str;
  MQ::OWO.AppendToCppString(&str);
  MQ::SERV_MESG.AppendToCppString(&str);
  const char *name = GetUsername();
  NQuad(static_cast<uint32_t>(strlen(name))).AppendToCppString(&str);
  str.append(name);
  uint32_t content_size = GetContent().size();
  NQuad(content_size).AppendToCppString(&str);
  str.append(GetContent());
  return buf->Add(str);
}
}  // namespace tin
