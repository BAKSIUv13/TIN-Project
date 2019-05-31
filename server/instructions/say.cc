// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <memory>

#include "instructions/say.h"

#include "send_msgs/user_msg.h"

#include "send_msgs/sig.h"

namespace tin {
int Say::Fn(Server *server, SocketStuff *stuff, World *world,
    MsgPushFn push_fn) {
  un_ = server->SockToUn(stuff->GetId());
  int pom;
  if (stuff->CmProcessed() < Msg_()) {
    pom = stuff->ReadQuad(MsgLen_(), &len_);
    if (pom != 0) {
      return pom;
    }
  }
  if (len_ > LEN_CUT) {
    (server->*push_fn)(OutMessage::UP(
      new Sig(stuff->GetId(), MQ::ERRR_LONG_MSG, true)));
    return -1;
  }
  if (stuff->CmProcessed() < End_()) {
    pom = stuff->ReadCpp11String(Msg_(), len_, &message_);
    if (pom != 0) return pom;
  }
  if (!un_) {
    LogM << "Gniazdo " << stuff->GetId() << " nie jest zalogowane, nie wyśle "
      << "wiadomości.\n";
    (server->*push_fn)(std::unique_ptr<OutMessage>(
      new Sig(stuff->GetId(), MQ::ERR_NOT_LOGGED, false)));
    return 0;
  }
  ChatMsg cmsg(un_, std::move(message_));
  world->PutMsg(std::move(cmsg));
  return 0;
}

}  // namespace tin
