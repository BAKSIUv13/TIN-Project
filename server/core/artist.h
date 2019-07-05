// Copyright 2019 TIN

#ifndef SERVER_CORE_ARTIST_H_
#define SERVER_CORE_ARTIST_H_

#include <string>

#include "image/image_types.h"
#include "core/username.h"

namespace tin {
class Artist {
 public:
  using MousePosition = std::array<double, 2>;
  using ObjectId = ImageTypes::ObjectId;

  explicit Artist(Username un) : un_(un) {}

  void SetCursor(double x, double y) {
    mouse_[0] = x;
    mouse_[1] = y;
  }

  // void Grab(ObjectId id) {
  //   selected_object_ = id;
  // }

 private:
  Username un_;
  MousePosition mouse_;
  // This will not be implemented yet.
  // ObjectId selected_object_;
};  // class Artist
}  // namespace tin

#endif  // SERVER_CORE_ARTIST_H_
