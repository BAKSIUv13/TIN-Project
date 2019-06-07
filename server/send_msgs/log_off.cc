// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <string>

#include "send_msgs/log_off.h"
#include "core/mquads.h"

namespace tin {
int LogOff::AddToBuf(WriteBuf *buf) const {
  std::string str;
  MQ::OWO.AppendToCppString(&str);
  MQ::SERV_LOG_OUT.AppendToCppString(&str);
  return buf->Add(str);
}
}  // namespace tin
