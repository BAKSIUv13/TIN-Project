// Copyright 2019 ja

#include "app/logged_user.h"

#include <unistd.h>

#include <set>
#include <vector>
#include <iostream>
#include <cstring>
#include <ios>
#include <utility>

#include "core/socket_tcp4.h"

namespace tin {

LoggedUser::LoggedUser(const Username &name, SessionId sid)
    : LoggedUser() {
  if (Username::GOOD & name.GetState()) {
    session_id_ = sid;
    name_ = name;
  }
}

LoggedUser::LoggedUser(LoggedUser &&other) {
  Move_(&other);
}

LoggedUser &LoggedUser::operator=(LoggedUser &&other) {
  if (this != &other) {
    Move_(&other);
  }
  return *this;
}

void LoggedUser::Clear_() {
  name_ = Username();
  session_id_ = 0;
  sock_fd_ = -1;
  artist_id_ = 0;
}

void LoggedUser::Move_(LoggedUser *other) {
  name_ = other->name_;
  session_id_ = other->session_id_;
  sock_fd_ = other->sock_fd_;
  artist_id_ = other->artist_id_;
  other->Clear_();
}

}  // namespace tin
