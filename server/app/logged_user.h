// Copyright 2019 Piotrek

#ifndef SERVER_APP_LOGGED_USER_H_
#define SERVER_APP_LOGGED_USER_H_

#include <string>

#include "app/session.h"
#include "core/username.h"
#include "core/sock_id.h"

namespace tin {
class LoggedUser{
 public:
  constexpr LoggedUser() : sock_id_(0) {}
  explicit LoggedUser(const Username &, SockId);
  LoggedUser(const LoggedUser &) = delete;
  LoggedUser(LoggedUser &&);
  LoggedUser &operator=(LoggedUser &&);

  const Username &GetName() const {return name_;}
  int GetSockId() const {return sock_id_;}
  void SetSockId(int id) {sock_id_ = id;}
  void ClrSock() {sock_id_ = 0;}
 private:
  void Move_(LoggedUser *);
  void Clear_();

  Username name_;  // nadmiar, bo będzie też wyszukiwanie po numerze sesji
  SockId sock_id_;
};  // class LoggedUser
}  // namespace tin

#endif  // SERVER_APP_LOGGED_USER_H_
