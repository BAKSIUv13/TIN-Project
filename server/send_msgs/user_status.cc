// Copyright 2019 Piotrek

#include <iostream>
#include <utility>

#include "send_msgs/user_status.h"
#include "core/mquads.h"

namespace tin {

int UserStatus::AddToBuf(WriteBuf *buf) {
  std::string str;
  MQ::OWO.AppendToCppString(&str);
  MQ::SERV_USER_STATUS.AppendToCppString(&str);
  what_.AppendToCppString(&str);
  NQuad(GetUsername().Len());
  str.append(GetUsername());
  return buf->Add(str);
}
}  // namespace tin
