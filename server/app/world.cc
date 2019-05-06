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

int World::PushMsg(OutMessage *msg) {
  messages_to_send_.push_back(*msg);
  return 0;
}

int World::AddArtist(const Username& un) {
  artists_[un];
  return 0;
}

OutMessage *World::FirstMsg() {
  if (messages_to_send_.size() < 1) return nullptr;
  return &messages_to_send_[0];
}

int World::PopMsg() {
  messages_to_send_.erase(messages_to_send_.begin());
  return 0;
}

}  // namespace tin
