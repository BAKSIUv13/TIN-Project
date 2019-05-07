// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <string>

#include "send_msgs/sess_capt_ok.h"
#include "core/mquads.h"

namespace tin {
int SessCaptOk::AddToBuf(WriteBuf *buf) {
  int NQS = sizeof(NQuad);
  std::string str;
  str.append(MQ::OWO.CStr(), NQS);
  str.append(MQ::SESSION_ACCEPTED.CStr(), NQS);
  return buf->Add(str);
}
}  // namespace tin
