// Copyright 2019 Piotrek

#include "core/nquad.h"
#include "core/mquads.h"

#include "instructions/log_in.h"
#include "instructions/log_out.h"
#include "instructions/say.h"
#include "instructions/move_mouse.h"
#include "instructions/create_shape.h"

namespace tin {

const std::map<InstrId, InstrSupp> Server::instructions {
  {InstrId(MQ::CLI_LOGIN)              , InstrStruct::Supp<LogIn>()      },
  {InstrId(MQ::CLI_LOGOUT)             , InstrStruct::Supp<LogOut>()     },
  {InstrId(MQ::CLI_MESG)               , InstrStruct::Supp<Say>()        },
  {InstrId(MQ::CLI_MOUSE)              , InstrStruct::Supp<MoveMouse>()  },
  {InstrId(MQ::CLI_CR_SH)              , InstrStruct::Supp<CreateShape>()},
};

}  // namespace tin
