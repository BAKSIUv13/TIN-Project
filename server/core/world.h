// Copyright 2019 Piotrek

#ifndef SERVER_CORE_WORLD_H_
#define SERVER_CORE_WORLD_H_

#include <map>
#include <vector>
#include <list>
#include <string>

#include "core/artist.h"
#include "image/image.h"
#include "core/out_message.h"
#include "chat/chat_msg.h"

namespace tin {
class World {
 public:
  World() : next_msg_(chat_msgs_.end()) {}

  int AddArtist(const Username &);
  int RemoveArtist(const Username &);
  int PutMsg(ChatMsg &&);
  int NextMsg(Username *, const std::string **);
  int SetCursor(const Username &, double x, double y);
 private:
  // To jest tylko dodatkowa struktura z myszką i tak dalej.
  std::map<Username, Artist> artists_;

  std::list<ChatMsg> chat_msgs_;
  decltype(chat_msgs_)::iterator next_msg_;  // to send

};  // class World
}  // namespace tin

#endif  // SERVER_CORE_WORLD_H_
