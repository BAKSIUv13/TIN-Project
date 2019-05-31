// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <memory>

#include "instructions/move_mouse.h"

#include "send_msgs/mouse_moved.h"
#include "send_msgs/sig.h"

namespace tin {
int MoveMouse::Fn(Server *server, SocketStuff *stuff, World *world,
    MsgPushFn push_fn) {
  MoveMouse *s = reinterpret_cast<MoveMouse *>(stuff->GetStrct());
  LogL << "Przesuwanie myszki.\n";
  s->un_ = server->SockToUn(stuff->GetId());
  int pom;
  if (stuff->CmProcessed() < s->Y) {
    pom = stuff->ReadDouble(s->X, &s->x_);
    if (pom != 0) return pom;
  }
  if (stuff->CmProcessed() < s->END) {
    pom = stuff->ReadDouble(s->Y, &s->y_);
    if (pom != 0) return pom;
  }
  if (!s->un_) {
    LogM << "Gniazdo " << stuff->GetId() << " nie jest zalogowane, nie przesuni"
      "e myszki.\n";
    (server->*push_fn)(std::unique_ptr<OutMessage>(
      new Sig(stuff->GetId(), MQ::ERR_NOT_LOGGED, false)));
    return -1;
  }

  LogM << "Ok, mam koordy: " << s->x_ << " " << s->y_
    << "\n";
  world->SetCursor(s->un_, s->x_.Double(), s->y_.Double());
  (server->*push_fn)
    (std::move(OutMessage::GenMsg(new MouseMoved(s->un_, s->x_, s->y_))));
  return 0;
}

void MoveMouse::Construct(InstrStruct *q) {
  new(q) MoveMouse();
}

void MoveMouse::Destroy(InstrStruct *q) {
  // reinterpret_cast<CaptureSession *>(q)->~CaptureSession();
  q->~InstrStruct();
  std::cerr << "move_mouse  : destroyv " << q << '\n';
}

const int MoveMouse::START, MoveMouse::END;

}  // namespace tin
