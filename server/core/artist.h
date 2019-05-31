// Copyright 2019 TIN

#ifndef SERVER_CORE_ARTIST_H_
#define SERVER_CORE_ARTIST_H_

#include <string>

#include "core/username.h"

namespace tin {
class Artist{
 public:
  using MousePosition = std::array<double, 2>;

  explicit Artist(Username un) : un_(un) {}

  void SetCursor(double x, double y) {
    mouse_[0] = x;
    mouse_[1] = y;
  }
 private:
  Username un_;
  MousePosition mouse_;
};  // class Artist
}  // namespace tin

#endif  // SERVER_CORE_ARTIST_H_
