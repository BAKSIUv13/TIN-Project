// Copyright 2019 Piotrek;

#ifndef SERVER_CORE_MQUADS_H_
#define SERVER_CORE_MQUADS_H_

#include "core/nquad.h"

namespace tin {
namespace MQ {

constexpr NQuad
  ZERO {},
  OWO {"OwO!"},
  // CAPTURE_SESSION {"sess"},
  // SESSION_ACCEPTED {"okej"},

  ERR_BAD_LOG {"badl"},
  ERR_BANNED {"band"},
  ERR_WAS_LOGGED {"wasl"},
  ERR_ACC_OCCUPIED {"acoc"},
  ERR_NOT_LOGGED {"nlog"},
  ERR_OTHER {"loer"},

  ERRR_LONG_UN {"LNun"},
  ERRR_LONG_PW {"LNpw"},

  CLI_LOGIN {"logo"},
  CLI_LOGOUT {"lout"},
  CLI_MOUSE {"maus"},
  CLI_MESG {"mesg"},


  MOVE_MOUSE {"mysz"},
  SAY {"msg0"},
  SAY2 {"sayy"},  // ??
  SERVER_DELIVER_MSG {"msg1"},
  SERVER_MOUSE {"maus"},
  SERVER_DELIVER_MSG2 {"said"},  // ??
  SERVER_DISCONNECT {"disc"},
  SERVER_ANNOUNCEMENT {"hejo"},
  SERVER_NOTICE {"ueuo"},

  SERV_LOGIN_OK {"LGOK"},
  SERV_LOG_OUT {"LOFF"},
  SERV_MOUSE {"MAUS"},
  SERV_MESG {"MESG"},

  SERV_SIGL {"SIGL"},
  SERV_SIGH {"SIGH"},

  RECTANGLE {"rect"},
  ELLIPSE {"elip"},
  POLYGONAL_CHAIN {"path"},

  EDIT {"edit"},
  ADD_PARAMS {"addd"},
  GRAB {"grab"},
  DONE {"done"},
  NOPE {"NOPE"},
  OBJECT_DELETED {"puff"};

}  // namespace MQ
}  // namespace tin

#endif  // SERVER_CORE_MQUADS_H_
