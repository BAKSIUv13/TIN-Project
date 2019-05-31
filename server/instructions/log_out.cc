// Copyright 2019 Piotrek

#include <iostream>
#include <memory>
#include <utility>

#include "instructions/log_out.h"

namespace tin {

int LogOut::Fn(Server *server, SocketStuff *stuff, World *, MsgPushFn push_fn) {
  // LogOut *s = reinterpret_cast<LogOut *>(stuff->GetStrct());
  int pom = server->LogOutUser(stuff->GetId(), true);
  if (pom < 0) {
    return 0;
  }
  return 0;
}

void LogOut::Construct(InstrStruct *q) {
  new(q) LogOut();
}

void LogOut::Destroy(InstrStruct *q) {
  // reinterpret_cast<LogIn *>(q)->~LogIn();
  q->~InstrStruct();
  std::cerr << "LogOut  : destroyv " << q << '\n';
}

}  // namespace tin
