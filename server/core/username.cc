// Copyright 0 0

#include "core/username.h"

#include <cstring>
#include <cctype>

namespace tin {

Username::Username() : state_(BLANK) {}

Username::Username(const Username &other) {
  memcpy(this, &other, sizeof(*this));
}

Username &Username::operator=(const Username &other) {
  if (this != &other) {
    memcpy(this, &other, sizeof(*this));
  }
  return *this;
}

Username::Username(const std::string &str) {
  if (str.size() > MAX_NAME_LEN) {
    state_ = BAD;
    return;
  }
  strncpy(c_, str.c_str(), MAX_NAME_LEN + 1);
  state_ = Check_();
}

bool Username::RawEqual_(const Username &other) const {
  if (state_ == BAD || other.state_ == BAD) {
    return false;
  }
  return state_ == other.state_
    && !strncmp(c_, other.c_, 16);
}

int Username::Check_() const {
  int not_spaces = 0;
  int chars = 0;
  int last_char = '\0';
  bool may_be_condensed = true;
  char c = c_[chars];
  unsigned char &u = c;
  if (std::isspace(c)) {
    return BAD;
  }
  while (c != '\0' && chars < 17) {
    if (std::isspace(c) && std::isspace(last_char)) {
      return BAD;
    }
    ++chars;
    last_char = c;
    c = c_[chars];

  }
}


}  // namespace tin
