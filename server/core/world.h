// Copyright 2019 Piotrek

#ifndef SERVER_CORE_WORLD_H_
#define SERVER_CORE_WORLD_H_

#include <map>
#include <vector>
#include <list>
#include <string>
#include <utility>

#include "core/artist.h"
#include "image/image.h"
#include "core/out_message.h"
#include "chat/chat_msg.h"

namespace tin {
class World {
 public:
  using ObjectId = ImageTypes::ObjectId;

  World() : next_msg_(chat_msgs_.end()) {}

  int AddArtist(const Username &);
  int RemoveArtist(const Username &);
  int PutMsg(ChatMsg &&);
  int NextMsg(Username *, const std::string **);

  int SetCursor(const Username &, double x, double y);

  template <typename T>
  std::pair<ObjectId, T *> AddObject(const Username &un) {
    if (artists_.count(un) < 1) {
      return std::pair<ObjectId, T *>(0, nullptr);
    }
    auto obj = image_.NewObject<T>();
    // TO DO usunąć stare zaznaczenie!!!
    artists_.at(un).Grab(obj.first);
    grabs_.emplace(obj.first, un);
    return std::make_pair(obj.first, &obj.second);
  }

  int RemoveObject(ObjectId);
/*
  template <typename T>
  T *GetObject(ObjectId id) {
    if (decltype(int) == decltype(int)) {
      return nullptr;
    }
  }

*/

  BasicObject *GetObject(ObjectId id) {
    return image_.GetObject(id);
  }

 private:
  // To jest tylko dodatkowa struktura z myszką i tak dalej.
  std::map<Username, Artist> artists_;
  Image image_;

  std::list<ChatMsg> chat_msgs_;
  decltype(chat_msgs_)::iterator next_msg_;  // to send

  std::map<ObjectId, Username> grabs_;
};  // class World
}  // namespace tin

#endif  // SERVER_CORE_WORLD_H_
