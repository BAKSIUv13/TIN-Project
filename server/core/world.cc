// Copyright 2019 Piotrek

#include "core/world.h"

#include <unistd.h>

#include <set>
#include <vector>
#include <iostream>
#include <cstring>
#include <ios>
#include <utility>

#include "core/socket_tcp4.h"

namespace tin {

int World::AddArtist(const Username &un) {
  artists_.emplace(un, un);
  return 0;
}

int World::RemoveArtist(const Username &un) {
  artists_.erase(un);
  return 0;
}

int World::NextMsg(Username *un_ptr, const std::string **str_ptr_ptr) {
  if (next_msg_ == chat_msgs_.end()) {
    *un_ptr = Username();
    *str_ptr_ptr = nullptr;
    return 1;
  }
  *un_ptr = next_msg_->GetAuthor();
  *str_ptr_ptr = &next_msg_->GetContent();
  next_msg_->MarkAsSent();
  ++next_msg_;
  return 0;
}

int World::SetCursor(const Username& un, double x, double y) {
  try {
    artists_.at(un).SetCursor(x, y);
  } catch(std::out_of_range &e) {
    return -1;
  }
  return 0;
}

int World::PutMsg(ChatMsg &&msg) {
  try {
    chat_msgs_.emplace_back(std::move(msg));
  } catch(std::bad_alloc &e) {
    return -1;
  }
  if (next_msg_ == chat_msgs_.end()) {
    --next_msg_;
  }
  return 0;
}

}  // namespace tin
