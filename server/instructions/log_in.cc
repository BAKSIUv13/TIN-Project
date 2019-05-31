// Copyright 2019 Piotrek

#include <iostream>
#include <memory>
#include <utility>

#include "instructions/log_in.h"

#include "send_msgs/sig.h"

namespace tin {

int LogIn::Fn(Server *server, SocketStuff *stuff, World *, MsgPushFn push_fn) {
  int pom;
  if (stuff->CmProcessed() < Un_()) {
    pom = stuff->ReadQuad(UnLen_(), &un_len_);
    if (pom != 0) {
      return pom;
    }
  }
  if (un_len_ > Server::MAX_UN_LEN) {
    (server->*push_fn)(OutMessage::UP(
      new Sig(stuff->GetId(), MQ::ERRR_LONG_UN, true)));
    return -1;
  }
  if (stuff->CmProcessed() < PwLen_()) {
    pom = stuff->ReadCpp11String(Un_(), un_len_, &un_);
    if (pom != 0) {
      return pom;
    }
  }
  if (stuff->CmProcessed() < Pw_()) {
    pom = stuff->ReadQuad(PwLen_(), &pw_len_);
    if (pom != 0) {
      return pom;
    }
  }
  if (pw_len_ > Server::MAX_PW_LEN) {
    (server->*push_fn)(OutMessage::UP(
      new Sig(stuff->GetId(), MQ::ERRR_LONG_PW, true)));
    return -1;
  }
  if (stuff->CmProcessed() < End_()) {
    pom = stuff->ReadCpp11String(Pw_(), pw_len_, &pw_);
    if (pom != 0) {
      return pom;
    }
  }
  pom = server->LogInUser(Username(un_), pw_, stuff->GetId(), true);
  if (pom < 0) {
    return 0;  // Chodzi o to, że po takim logowanku nie jest źle.
  }
  return 0;
}

}  // namespace tin
