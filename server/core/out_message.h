// Copyright 2019 Piotrek

#ifndef SERVER_CORE_OUT_MESSAGE_H_
#define SERVER_CORE_OUT_MESSAGE_H_

#include <string>
#include <list>

#include "core/username.h"
#include "core/write_buf.h"
#include "core/sock_id.h"

namespace tin {

class OutMessage {
 public:
  enum : int {
    // Send to all sockets.
    BROADCAST_S,
    // Send to all logged users.
    BROADCAST_U,
    // Send to listed sockets.
    LIST_S,
    // Send to listed logged users.
    LIST_U,
    // Send to one socket.
    ONE_S,
    // Send to one logged user.
    ONE_U,
  };
  OutMessage() {}
  virtual ~OutMessage() {}

  virtual std::string GetTypeName() = 0;

  virtual int Audience() = 0;

  // User list.
  virtual std::list<Username> Users();

  // Socket list to send.
  virtual std::list<SockId> Socks();

  virtual Username User();

  virtual SockId Sock();

  virtual int AddToBuf(WriteBuf *) = 0;
};
}  // namespace tin

#endif  // SERVER_CORE_OUT_MESSAGE_H_
