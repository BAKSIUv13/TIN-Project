// Copyright 2019 Piotrek

#include <iostream>
#include <memory>
#include <utility>

#include "instructions/log_out.h"

namespace tin {

int LogOut::Fn(Server *server, SocketStuff *stuff, World *) {
  int pom = server->LogOutUser(stuff->GetId(), true);
  if (pom < 0) {
    return 0;
  }
  return 0;
}

}  // namespace tin
