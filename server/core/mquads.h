// Copyright 2019 Piotrek;

#ifndef SERVER_CORE_MQUADS_H_
#define SERVER_CORE_MQUADS_H_

#include "core/nquad.h"

namespace tin {
namespace MQ {

constexpr NQuad
  ZERO {},
  OWO {"OwO!"},
  CAPTURE_SESSION {"sess"},
  REQUEST_LOGIN {"logi"},
  SEND_MESSAGE {"msg0"},
  SERVER_DELIVER_MSG {"msg1"},
  SERVER_DISCONNECT {"disc"},
  SERVER_ANNOUNCEMENT {"hejo"},
  SERVER_INFO {"ueuo"};
}  // namespace MQ
}  // namespace tin

#endif  // SERVER_CORE_MQUADS_H_
