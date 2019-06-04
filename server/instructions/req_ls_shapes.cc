// Copyright 2019 Piotrek

#include <iostream>
#include <memory>
#include <utility>

#include "instructions/req_ls_shapes.h"
#include "send_msgs/list_shapes.h"
#include "send_msgs/sig.h"

namespace tin {

int ReqLsShapes::Fn(Server *server, SocketStuff *stuff, World *world) {
  Username un = server->SockToUn(stuff->GetId());
  if (un) {
    server->PushMsg<ListShapes>(un, world->GetShapeIterators());
  } else {
    server->PushMsg<Sig>(stuff->GetId(), MQ::ERR_NOT_LOGGED, false);
  }
  return 0;
}

}  // namespace tin
