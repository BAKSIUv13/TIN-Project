// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <string>

#include "send_msgs/sig.h"
#include "core/mquads.h"

namespace tin {
int Sig::AddToBuf(WriteBuf *buf) {
  int NQS = sizeof(NQuad);
  std::string str;
  str.append(MQ::OWO.CStr(), NQS);
  if (IsHeavy()) {
    str.append(MQ::SERV_SIGH, NQS);
  } else {
    str.append(MQ::SERV_SIGL, NQS);
  }
  str.append(code_, NQS);
  str.append(NQuad(static_cast<uint32_t>(msg_.size())), NQS);
  str.append(msg_);
  return buf->Add(str);
}
}  // namespace tin
