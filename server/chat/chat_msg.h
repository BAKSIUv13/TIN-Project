// Copyright 2019 Piotrek

#ifndef SERVER_CHAT_CHAT_MSG_H_
#define SERVER_CHAT_CHAT_MSG_H_

#include <iostream>
#include <string>
#include <utility>

#include "core/nquad.h"
#include "core/username.h"

namespace tin {
class ChatMsg {
 public:
  ChatMsg(const Username &un, std::string &&content)
      : content_(std::move(content)), author_(un) {}

  void MarkAsSent() {is_sent_ = true;}
  const std::string &GetContent() {return content_;}
  Username GetAuthor() {return author_;}
 private:
  std::string content_;
  Username author_;
  bool is_sent_;
};

}  // namespace tin

#endif  // SERVER_CHAT_CHAT_MSG_H_
