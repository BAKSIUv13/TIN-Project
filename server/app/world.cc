// Copyright 2019 Piotrek

#include "app/world.h"

#include <unistd.h>

#include <set>
#include <vector>
#include <iostream>
#include <cstring>
#include <ios>
#include <utility>

#include "core/socket_tcp4.h"

namespace tin {

int World::AddArtist(const Username& un) {
  artists_[un];
  return 0;
}

}  // namespace tin
