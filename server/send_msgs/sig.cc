// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <string>

#include "send_msgs/sig.h"
#include "core/mquads.h"

namespace tin {
int Sig::AddToBuf(WriteBuf *buf) {
  std::string str;
  MQ::OWO.AppendToCppString(&str);
  if (IsHeavy()) {
    MQ::SERV_SIGH.AppendToCppString(&str);
  } else {
    MQ::SERV_SIGL.AppendToCppString(&str);
  }
  code_.AppendToCppString(&str);
  NQuad(static_cast<uint32_t>(msg_.size())).AppendToCppString(&str);
  str.append(msg_);
  return buf->Add(str);
}
}  // namespace tin
