// Copyright 2019 Piotrek

#ifndef SERVER_APP_WORLD_H_
#define SERVER_APP_WORLD_H_

#include <map>
#include <vector>

#include "app/artist.h"
#include "image/image.h"
#include "app/out_message.h"

namespace tin {
class World {
 public:
  int AddArtist(const Username&);
  int RemoveArtist();
  int PushMsg(OutMessage *);
  OutMessage *FirstMsg();
  int PopMsg();
 private:
  std::map<Username, Artist> artists_;
  std::vector<OutMessage> messages_to_send_;
};  // class World
}  // namespace tin

#endif  // SERVER_APP_WORLD_H_
