// Copyright 2019 Piotrek;

#ifndef SERVER_CORE_MQUADS_H_
#define SERVER_CORE_MQUADS_H_

#include "core/nquad.h"

namespace tin {
namespace MQ {

constexpr NQuad
  ZERO {},
  OWO {"OwO!"},

  ERR_BAD_LOG {"badl"},
  ERR_BANNED {"band"},
  ERR_WAS_LOGGED {"wasl"},
  ERR_ACC_OCCUPIED {"acoc"},
  ERR_NOT_LOGGED {"nlog"},
  ERR_OTHER {"loer"},

  ERRR_LONG_UN {"LNun"},
  ERRR_LONG_PW {"LNpw"},
  ERRR_LONG_MSG {"LNms"},

  CLI_LOGIN {"logo"},
  CLI_LOGOUT {"lout"},
  CLI_MOUSE {"maus"},
  CLI_MESG {"mesg"},

  SERV_LOGIN_OK {"LGOK"},
  SERV_LOG_OUT {"LOFF"},
  SERV_MOUSE {"MAUS"},
  SERV_MESG {"MESG"},

  SERV_SIGL {"SIGL"},
  SERV_SIGH {"SIGH"},

  SHAPE_RECTANGLE {"rect"},
  SHAPE_ELLIPSE {"elip"},
  SHAPE_POLYGONAL_CHAIN {"path"},

  NOTHING {"xdxd"};

}  // namespace MQ
}  // namespace tin

#endif  // SERVER_CORE_MQUADS_H_
