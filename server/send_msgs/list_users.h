// Copyright 2019 Piotrek

#ifndef SERVER_SEND_MSGS_LIST_USERS_H_
#define SERVER_SEND_MSGS_LIST_USERS_H_

#include <string>
#include <list>
#include "core/out_message.h"
#include "core/write_buf.h"

namespace tin {

// This is class of message that shall be broadcast.
class ListUsers : public OutMessage {
 public:
  ListUsers() {}
  explicit ListUsers(const Username &un, std::list<Username> &&users)
      : username_(un), users_(users) {}

  virtual ~ListUsers() {}

  virtual std::string GetTypeName() const {return "ListUsers";}

  virtual int Audience() const {return ONE_U;}

  Username User() const override {
    return username_;
  }

  virtual std::string GetStr() const;

 private:
  Username username_;
  std::list<Username> users_;
};  // class ListUsers
}  // namespace tin

#endif  // SERVER_SEND_MSGS_LIST_USERS_H_
