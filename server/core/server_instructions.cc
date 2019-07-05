// Copyright 2019 Piotrek

#include "core/nquad.h"
#include "core/mquads.h"

#include "instructions/log_in.h"
#include "instructions/log_out.h"
#include "instructions/say.h"
#include "instructions/move_mouse.h"
#include "instructions/create_shape.h"
#include "instructions/req_ls_shapes.h"
#include "instructions/req_ls_users.h"

namespace tin {

const std::map<InstrId, InstrSupp> Server::instructions {
  {InstrId(MQ::CLI_LOGIN)              , InstrStruct::Supp<LogIn>()      },
  {InstrId(MQ::CLI_LOGOUT)             , InstrStruct::Supp<LogOut>()     },
  {InstrId(MQ::CLI_MESG)               , InstrStruct::Supp<Say>()        },
  {InstrId(MQ::CLI_MOUSE)              , InstrStruct::Supp<MoveMouse>()  },
  {InstrId(MQ::CLI_CR_SH)              , InstrStruct::Supp<CreateShape>()},
  {InstrId(MQ::CLI_LS_SHAPES)          , InstrStruct::Supp<ReqLsShapes>()},
  {InstrId(MQ::CLI_LS_USERS)           , InstrStruct::Supp<ReqLsUsers>() },
};

}  // namespace tin
