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
  ERRR_KICK {"KICK"},

  CLI_LOGIN {"logo"},
  CLI_LOGOUT {"lout"},
  CLI_MOUSE {"maus"},
  CLI_MESG {"mesg"},
  CLI_CR_SH {"crea"},
  CLI_LS_USERS {"lsus"},
  CLI_LS_SHAPES {"lssh"},

  SERV_LOGIN_OK {"LGOK"},
  SERV_LOG_OUT {"LOFF"},
  SERV_MOUSE {"MAUS"},
  SERV_MESG {"MESG"},
  SERV_NEW_OBJECT {"NEWW"},
  SERV_USER_STATUS {"USER"},
  SERV_LS_USERS {"LSUS"},
  SERV_LS_SHAPES {"LSSH"},
  SERV_CLEAR_CANVAS {"CLRR"},

  SERV_SIGL {"SIGL"},
  SERV_SIGH {"SIGH"},

  USER_LOGGED_IN {"Ulin"},
  USER_LOGGED_IN {"Ulof"},

  SHAPE_RECTANGLE {"rect"},
  SHAPE_ELLIPSE {"elip"},
  SHAPE_POLYGONAL_CHAIN {"path"},

  NOTHING {"xdxd"};

}  // namespace MQ
}  // namespace tin

#endif  // SERVER_CORE_MQUADS_H_
