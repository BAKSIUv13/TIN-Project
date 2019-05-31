// Copyright 2019 Piotrek

#ifndef SERVER_APP_WORLD_H_
#define SERVER_APP_WORLD_H_

#include <map>
#include <vector>
#include <list>

#include "app/artist.h"
#include "image/image.h"
#include "core/out_message.h"
#include "chat/chat_msg.h"

namespace tin {
class World {
 public:
  int AddArtist(const Username&);
  int RemoveArtist();
 private:
  // To jest tylko dodatkowa struktura z myszką i tak dalej.
  std::map<Username, Artist> artists_;

  std::list<ChatMsg> chat_msgs_;

};  // class World
}  // namespace tin

#endif  // SERVER_APP_WORLD_H_
