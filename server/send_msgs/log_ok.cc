// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <string>

#include "send_msgs/log_ok.h"
#include "core/mquads.h"

namespace tin {
int LogOk::AddToBuf(WriteBuf *buf) {
  std::string str;
  MQ::OWO.AppendToCpp11String(&str);
  MQ::SERV_LOGIN_OK.AppendToCpp11String(&str);
  return buf->Add(str);
}
}  // namespace tin
