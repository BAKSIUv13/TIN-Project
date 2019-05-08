// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <memory>

#include "instructions/move_mouse.h"

#include "send_msgs/mouse_moved.h"

namespace tin {

int MoveMouse::Fn(Server *server, int fd, SocketStuff *stuff, World *world,
    MsgPushFn push_fn) {
  MoveMouse *s = reinterpret_cast<MoveMouse *>(stuff->strct);
  int chars_to_copy;
  s->un_ = server->SockToUn(fd);
  std::cerr << "Przyszło przesuwanko myszki";
  if (s->un_.Good()) {
    std::cerr << " od gracza [[" << s->un_ << "]]\n";
  } else {
    std::cerr << " od kogoś nie zalogowanego xd\n";
    return -1;
  }
  if (stuff->cm_processed < END) {
    chars_to_copy = stuff->CountCopy(END);
    stuff->Copy(&s->coords_[stuff->cm_processed - START], chars_to_copy);
    if (stuff->cm_processed < END) {
      return 1;
    }
  }
  std::cerr << "Ok, mam koordy: " << s->x() << " " << s->y()
    << "\n";
  std::unique_ptr<OutMessage> msg {new MouseMoved(s->un_, s->x(), s->y())};
  (server->*push_fn)(std::move(msg));
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
