// Copyright 2019 Piotrek

#ifndef SERVER_APP_OUT_MESSAGE_H_
#define SERVER_APP_OUT_MESSAGE_H_

#include <string>

#include "core/username.h"

namespace tin {

// This is class of message that shall be broadcast.
class OutMessage {
 public:
  enum Type {
    BLANK,  // def constructor
    ANNOUNCEMENT,
    SERVER_NOTICE,
    USER_MESSAGE,
  };
  OutMessage() : type_(BLANK) {}
  OutMessage(const Username &un, const std::string &content);
  explicit OutMessage(const std::string &content);

  Type GetType() {return type_;}
  Username GetUsername() {return username_;}
  const std::string &GetContent() {return content_;}
 private:
  Type type_;
  Username username_;
  std::string content_;
};
}  // namespace tin

#endif  // SERVER_APP_OUT_MESSAGE_H_
