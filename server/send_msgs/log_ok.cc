// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <string>

#include "send_msgs/log_ok.h"
#include "core/mquads.h"

namespace tin {
std::string LogOk::GetStr() const {
  std::string str;
  MQ::OWO.AppendToCppString(&str);
  MQ::SERV_LOGIN_OK.AppendToCppString(&str);
  return str;
}
}  // namespace tin
