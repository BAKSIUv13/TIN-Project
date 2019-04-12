// Copyright 2077 jabka

#ifndef SERVER_APP_ARTIST_H_
#define SERVER_APP_ARTIST_H_

#include <string>

namespace tin {
class Artist{
 public:
  using MousePosition = std::array<int, 2>;

 private:
  MousePosition mouse_;
  int image_id_;
  std::string user_;
};  // class Artist
}  // namespace tin

#endif  // SERVER_APP_ARTIST_H_
