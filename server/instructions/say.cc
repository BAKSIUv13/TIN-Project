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
  Say *s = reinterpret_cast<Say *>(stuff->GetStrct());
  s->un_ = server->SockToUn(stuff->GetId());
  int pom;
  if (stuff->CmProcessed() < s->Msg_()) {
    pom = stuff->ReadQuad(s->MsgLen_(), &s->len_);
    if (pom != 0) {
      return pom;
    }
  }
  if (s->len_ > LEN_CUT) {
    (server->*push_fn)(std::unique_ptr<OutMessage>(
      new Sig(stuff->GetId(), MQ::ERRR_LONG_MSG, true)));
    return -1;
  }
  if (stuff->CmProcessed() < s->End_()) {
    pom = stuff->ReadCpp11String(s->Msg_(), s->len_, &s->message_);
    if (pom != 0) return pom;
  }
  if (!s->un_) {
    LogM << "Gniazdo " << stuff->GetId() << " nie jest zalogowane, nie wyśle "
      << "wiadomości.\n";
    (server->*push_fn)(std::unique_ptr<OutMessage>(
      new Sig(stuff->GetId(), MQ::ERR_NOT_LOGGED, false)));
    return 0;
  }
  ChatMsg cmsg(s->un_, std::move(s->message_));
  world->PutMsg(std::move(cmsg));
  return 0;
}

void Say::Construct(InstrStruct *q) {
  new(q) Say();
}

void Say::Destroy(InstrStruct *q) {
  // reinterpret_cast<CaptureSession *>(q)->~CaptureSession();
  q->~InstrStruct();
  LogM << "say: destroy " << q << '\n';
}

}  // namespace tin
