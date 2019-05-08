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
  SESSION_ACCEPTED {"okej"},
  REQUEST_LOGIN {"logi"},
  MOVE_MOUSE {"mysz"},
  SAY {"msg0"},
  SAY2 {"sayy"},  // ??
  SERVER_DELIVER_MSG {"msg1"},
  SERVER_MOUSE {"maus"},
  SERVER_DELIVER_MSG2 {"said"},  // ??
  SERVER_DISCONNECT {"disc"},
  SERVER_ANNOUNCEMENT {"hejo"},
  SERVER_NOTICE {"ueuo"};
}  // namespace MQ
}  // namespace tin

#endif  // SERVER_CORE_MQUADS_H_
