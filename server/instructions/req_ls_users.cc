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
    std::list<Username> user_list;
    for (auto it = server->usercbegin(); it != server->usercend(); ++it) {
      user_list.push_back(*it);
    }
    int res = server->PushMsg<ListUsers>(un, std::move(user_list));
    if (res < 0) {
      LogH << "lsus - chyba się pamięć kończy\n";
      return -1;
    }
  } else {
    server->PushMsg<Sig>(stuff->GetId(), MQ::ERR_NOT_LOGGED, false);
  }
  return 0;
}

}  // namespace tin
