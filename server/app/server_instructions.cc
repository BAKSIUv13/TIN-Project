// Copyright 2019 Piotrek

#include "core/nquad.h"
#include "core/mquads.h"

#include "instructions/log_in.h"
#include "instructions/log_out.h"
#include "instructions/say.h"
#include "instructions/move_mouse.h"

namespace tin {

const std::map<InstrId, InstrSupp> Server::instructions {
  {InstrId(MQ::CLI_LOGIN),
    InstrSupp(
      &LogIn::Fn,
      sizeof(LogIn),
      &LogIn::Construct,
      &LogIn::Destroy)},

  {InstrId(MQ::CLI_LOGOUT),
    InstrSupp(
      &LogOut::Fn,
      sizeof(LogOut),
      &LogOut::Construct,
      &LogOut::Destroy)},

  {InstrId(MQ::CLI_MESG),
    InstrSupp(
      &Say::Fn,
      sizeof(Say),
      &Say::Construct,
      &Say::Destroy)},
};

}  // namespace tin
