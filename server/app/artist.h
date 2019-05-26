// Copyright 2077 jabka

#ifndef SERVER_APP_ARTIST_H_
#define SERVER_APP_ARTIST_H_

#include <string>

#include "core/username.h"

namespace tin {
class Artist{
 public:
  using MousePosition = std::array<double, 2>;

 private:
  Username un_;
  MousePosition mouse_;
};  // class Artist
}  // namespace tin

#endif  // SERVER_APP_ARTIST_H_
