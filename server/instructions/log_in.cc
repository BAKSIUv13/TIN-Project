// Copyright 2019 Piotrek

#include <iostream>
#include <memory>
#include <utility>

#include "instructions/log_in.h"

#include "send_msgs/log_ok.h"

namespace tin {

int LogIn::Fn(Server *server, SocketStuff *stuff, World *, MsgPushFn push_fn) {
  LogIn *s = reinterpret_cast<LogIn *>(stuff->GetStrct());
  int pom;
  if (stuff->CmProcessed() < s->Un_()) {
    pom = stuff->ReadQuad(s->UnLen_(), &s->un_len_);
    if (pom != 0) {
      return pom;
    }
  }
  if (stuff->CmProcessed() < s->PwLen_()) {
    pom = stuff->ReadCpp11String(s->Un_(), s->un_len_, &s->un_);
    if (pom != 0) {
      return pom;
    }
  }
  if (stuff->CmProcessed() < s->Pw_()) {
    pom = stuff->ReadQuad(s->PwLen_(), &s->pw_len_);
    if (pom != 0) {
      return pom;
    }
  }
  if (stuff->CmProcessed() < s->End_()) {
    pom = stuff->ReadCpp11String(s->Pw_(), s->pw_len_, &s->pw_);
    if (pom != 0) {
      return pom;
    }
  }
  pom = server->LogInUser(Username(s->un_), s->pw_, stuff->GetId(), true);
  if (pom < 0) {
    return 0;  // Chodzi o to, że po takim logowanku nie jest źle.
  }
  return 0;
}

void LogIn::Construct(InstrStruct *q) {
  new(q) LogIn();
}

void LogIn::Destroy(InstrStruct *q) {
  // reinterpret_cast<LogIn *>(q)->~LogIn();
  q->~InstrStruct();
  std::cerr << "LogIn  : destroyv " << q << '\n';
}


}  // namespace tin
