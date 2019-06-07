// Copyright 2019 Piotrek

#ifndef SERVER_SEND_MSGS_USER_STATUS_H_
#define SERVER_SEND_MSGS_USER_STATUS_H_

#include <string>
#include <list>

#include "core/out_message.h"
#include "core/write_buf.h"

namespace tin {

// This is class of message that shall be broadcast.
class UserStatus : public OutMessage {
 public:
  UserStatus() {}
  UserStatus(const Username &un, NQuad w)
      : username_(un), what_(w) {}

  virtual ~UserStatus() {}

  virtual std::string GetTypeName() const {return "UserStatus";}

  virtual int Audience() const {return BROADCAST_U;}

  Username GetUsername() const {return username_;}
  constexpr NQuad What() const {return what_;}

  virtual std::string GetStr() const;
 private:
  Username username_;
  NQuad what_;
};  // class UserStatus
}  // namespace tin

#endif  // SERVER_SEND_MSGS_USER_STATUS_H_
