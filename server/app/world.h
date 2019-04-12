// Copyright 2077 jabka

#ifndef SERVER_APP_WORLD_H_
#define SERVER_APP_WORLD_H_

#include <map>

#include "app/artist.h"
#include "image/image.h"

namespace tin {
class World{
 public:
  using ArtistTable = std::map<int, Artist>;
  using ImageTable = std::map<int, Image>;

  int AddArtist();
  void RemoveArtist();
 private:
  ImageTable images_;
  ArtistTable artists_;
};  // class World
}  // namespace tin

#endif  // SERVER_APP_WORLD_H_
