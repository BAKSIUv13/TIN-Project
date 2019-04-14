// Copyright 2077 jabka

#ifndef SERVER_APP_LOGGED_USER_H_
#define SERVER_APP_LOGGED_USER_H_

#include <string>

#include "app/session.h"
#include "core/username.h"

namespace tin {
class LoggedUser{
 public:
  constexpr static int MAX_ARTIST_ID = 1000;
 private:
  Username name_;  // nadmiar, bo będzie też wyszukiwanie po numerze sesji
  SessionId session_id_;  // nr sesji 0 to jej brak
  int sock_fd_;
  int artist_id_;  // 0 - nie ma artysty
};  // class LoggedUser
}  // namespace tin

#endif  // SERVER_APP_LOGGED_USER_H_
