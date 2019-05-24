// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <string>

#include "send_msgs/log_ok.h"
#include "core/mquads.h"

namespace tin {
int LogOk::AddToBuf(WriteBuf *buf) {
  int NQS = sizeof(NQuad);
  std::string str;
  str.append(MQ::OWO.CStr(), NQS);
  str.append(MQ::SERV_LOGIN_OK.CStr(), NQS);
  return buf->Add(str);
}
}  // namespace tin
