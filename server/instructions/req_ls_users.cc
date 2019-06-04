// Copyright 2019 Piotrek

#include <iostream>
#include <memory>
#include <utility>

#include "instructions/req_ls_users.h"
#include "send_msgs/list_users.h"
#include "send_msgs/sig.h"

namespace tin {

int ReqLsUsers::Fn(Server *server, SocketStuff *stuff, World *) {
  Username un = server->SockToUn(stuff->GetId());
  if (un) {
    ListUsers *lu = server->PushMsg<ListUsers>(un);
    if (lu == nullptr) {
      LogH << "lsus - chyba się pamięć kończy\n";
      return -1;
    }
    for (auto it = server->usercbegin(); it != server->usercend(); ++it) {
      lu->AddUser(*it);
    }
  } else {
    server->PushMsg<Sig>(stuff->GetId(), MQ::ERR_NOT_LOGGED, false);
  }
  return 0;
}

}  // namespace tin
