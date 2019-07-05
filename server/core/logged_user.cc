// Copyright 2019 TIN

#include "core/logged_user.h"

#include <unistd.h>

#include <set>
#include <vector>
#include <iostream>
#include <cstring>
#include <ios>
#include <utility>

#include "core/socket_tcp4.h"

namespace tin {

LoggedUser::LoggedUser(const Username &name, SockId sid, Mode mode)
    : sock_id_(sid), mode_(mode) {
  if (Username::GOOD & name.GetState()) {
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
  sock_id_ = 0;
}

void LoggedUser::Move_(LoggedUser *other) {
  name_ = other->name_;
  sock_id_ = other->sock_id_;
  other->Clear_();
}

}  // namespace tin
