// Copyright 2019 Piotrek

#ifndef SERVER_CORE_INSTR_FN_H_
#define SERVER_CORE_INSTR_FN_H_

#include <memory>

// #include "core/out_message.h"

namespace tin {
class Server;
class World;
class SocketTCP4;
struct SocketStuff;
class OutMessage;
using MsgPushFn = int(Server::*)(std::unique_ptr<OutMessage>);
// Fn pointer because we do not need std::function and we can use 'constexpr'.
using InstrFn = int(*)(Server *, SocketStuff *, World *, MsgPushFn);
// using InstrFn = int(*)(Server *, int fd, SocketStuff *, World *, MsgPushFn);
}  // namespace tin

#endif  // SERVER_CORE_INSTR_FN_H_
