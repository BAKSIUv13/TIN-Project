// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <memory>

#include "instructions/move_mouse.h"

#include "send_msgs/mouse_moved.h"
#include "send_msgs/sig.h"

namespace tin {
int MoveMouse::Fn(Server *server, SocketStuff *stuff, World *world) {
  LogL << "Przesuwanie myszki.\n";
  un_ = server->SockToUn(stuff->GetId());
  int pom;
  if (stuff->CmProcessed() < Y) {
    pom = stuff->ReadDouble(X, &x_);
    if (pom != 0) return pom;
  }
  if (stuff->CmProcessed() < END) {
    pom = stuff->ReadDouble(Y, &y_);
    if (pom != 0) return pom;
  }
  if (!un_) {
    LogM << "Gniazdo " << stuff->GetId() << " nie jest zalogowane, nie przesuni"
      "e myszki.\n";
    server->PushMsg<Sig>(stuff->GetId(), MQ::ERR_NOT_LOGGED, false);
    return 0;
  }

  LogM << "Mam koordy: " << x_ << " " << y_
    << "\n";
  world->SetCursor(un_, x_.Double(), y_.Double());
  server->PushMsg<MouseMoved>(un_, x_, y_);
  return 0;
}


const int MoveMouse::START, MoveMouse::END;

}  // namespace tin
