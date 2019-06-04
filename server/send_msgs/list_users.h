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
  explicit ListUsers(const Username &un)
      : username_(un) {}

  virtual ~ListUsers() {}

  virtual std::string GetTypeName() {return "ListUsers";}

  virtual int Audience() {return ONE_U;}

  void AddUser(Username un) {
    users_.push_back(un);
  }

  const std::list<Username> GetUsers() {
    return users_;
  }

  Username User() override {
    return username_;
  }

  virtual int AddToBuf(WriteBuf *buf);

 private:
  Username username_;
  std::list<Username> users_;
};  // class ListUsers
}  // namespace tin

#endif  // SERVER_SEND_MSGS_LIST_USERS_H_
