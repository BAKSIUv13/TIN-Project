// Copyright 0 0

#include "core/username.h"

#include <cstring>
#include <cctype>
#include <locale>

namespace tin {
/*
Username::Username() : state_(BLANK) {
  c_[0] = '\0';
}
*/
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
  } else {
    strncpy(c_, str.c_str(), MAX_NAME_LEN + 1);
    state_ = Check_();
  }
  ZeroBad_();
}

Username::Username(const char *c) {
  strncpy(c_, c, MAX_NAME_LEN + 1);
  state_ = Check_();
  ZeroBad_();
}

int Username::RawCompare_(const Username &other) const {
  if (state_ == BAD || other.state_ == BAD) {
    return false;
  }
  return strncmp(c_, other.c_, MAX_NAME_LEN + 1);
}

int Username::Check_() const {
  int not_spaces = 0;
  int chars = 0;
  bool condensed = true;
  char c = c_[chars];
  //  char last_c = '\0';
  unsigned char u = c;
  unsigned char last_u = '\0';
  if (c == '\0') {
    return BLANK;
  }
  if (std::isspace(u)) {
    return BAD;
  }
  while (c != '\0' && chars <= MAX_NAME_LEN) {
    if (std::isspace(u)
      && std::isspace(last_u)) {
      return BAD;
    }
    if (!(std::isalnum(u) || c == ' ' || c == '_')) {
      return BAD;
    }
    if (std::isupper(u) || c == ' ') {
      condensed = false;
    }
    if (!std::isspace(u)) {
      ++not_spaces;
    }
    ++chars;
    //  last_c = c;
    last_u = u;
    c = c_[chars];
    u = c;
  }
  if (std::isspace(last_u)) {
    return BAD;
  }
  if (c_[chars] != '\0') {
    return BAD;
  }
  if (not_spaces < 3) {
    return BAD;
  }
  return condensed ? CONDENSED : GOOD;
}

Username::Username(const Username &model, Username::CondenseFlag_t) {
  switch (model.state_) {
    case BLANK:
      state_ = BLANK;
      c_[0] = '\0';
      return;
    case CONDENSED:
      state_ = CONDENSED;
      strncpy(c_, model.c_, MAX_NAME_LEN + 1);
      return;
    case GOOD: {
        bool error = false;
        int model_it = 0;
        int this_it = 0;
        char c = model.c_[0];
        unsigned char u = c;
        while (c != '\0' && model_it <= MAX_NAME_LEN) {
          if (std::isupper(u)) {
            c_[this_it] = std::tolower(u);
            ++this_it;
          } else if (c == ' ') {
            // do nothing :>
          } else if (std::isalnum(u) || c == '_') {
            c_[this_it] = c;
            ++this_it;
          } else {
            error = true;
            break;  // break loop not switch
          }
          ++model_it;
          c = model.c_[model_it];
          u = c;
        }
        c_[this_it] = '\0';
        state_ = error ? BAD : CONDENSED;
        return;
      }
    case BAD: default:
      state_ = BAD;
      c_[0] = '\0';
      return;
  }
}

int Username::Compare_(const Username &other) const {
  std::array<Username, 2> o;
  const Username *q[] = {this, &other};
  for (size_t i = 0; i < 2; ++i) {
    if (q[i]->state_ == GOOD) {
      o[i] = Username(*q[i], CondenseFlag);
      q[i] = &o[i];
    }
  }
  return q[0]->RawCompare_(*q[1]);
}


void Username::ZeroBad_() {
  if (state_ == BAD || state_ == BLANK) {
    c_[0] = '\0';
  }
}

}  // namespace tin
