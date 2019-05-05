// Copyright 2019 Piotrek

#ifndef SERVER_CORE_INSTR_FN_H_
#define SERVER_CORE_INSTR_FN_H_

namespace tin {
class Server;
class SocketTCP4;
struct SocketStuff;
// using FnType = std::function<int(Server *, int, SocketTCP4, SocketStuff)>;
// Fn pointer because we do not need std::function and we can use 'constexpr'.
using InstrFn = int(*)(Server *, int, SocketTCP4 *, SocketStuff *);
}  // namespace tin

#endif  // SERVER_CORE_INSTR_FN_H_
