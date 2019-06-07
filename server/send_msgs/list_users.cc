// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <string>

#include "send_msgs/list_users.h"
#include "core/mquads.h"

namespace tin {
int ListUsers::AddToBuf(WriteBuf *buf) const {
  std::string str;
  MQ::OWO.AppendToCppString(&str);
  MQ::SERV_LS_USERS.AppendToCppString(&str);
  NQuad(static_cast<uint32_t>(users_.size())).AppendToCppString(&str);
  for (auto &x : users_) {
    NQuad(x.Len()).AppendToCppString(&str);
    str.append(x);
  }
  return buf->Add(str);
}
}  // namespace tin
