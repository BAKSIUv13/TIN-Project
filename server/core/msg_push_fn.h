// Copyright 2019 Piotrek

#ifndef SERVER_CORE_MSG_PUSH_FN_H_
#define SERVER_CORE_MSG_PUSH_FN_H_

#include <memory>

namespace tin {
class Server;
class OutMessage;
using MsgPushFn = int(Server::*)(std::unique_ptr<OutMessage>);

}  // namespace tin

#endif  // SERVER_CORE_MSG_PUSH_FN_H_
