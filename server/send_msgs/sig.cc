// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <string>

#include "send_msgs/sig.h"
#include "core/mquads.h"

namespace tin {
int Sig::AddToBuf(WriteBuf *buf) {
  std::string str;
  MQ::OWO.AppendToCpp11String(&str);
  if (IsHeavy()) {
    MQ::SERV_SIGH.AppendToCpp11String(&str);
  } else {
    MQ::SERV_SIGL.AppendToCpp11String(&str);
  }
  code_.AppendToCpp11String(&str);
  NQuad(static_cast<uint32_t>(msg_.size())).AppendToCpp11String(&str);
  str.append(msg_);
  return buf->Add(str);
}
}  // namespace tin
