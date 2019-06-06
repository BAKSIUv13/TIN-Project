// Copyright 2019 TIN

#ifndef SERVER_CORE_LOGGED_USER_H_
#define SERVER_CORE_LOGGED_USER_H_

#include <string>

#include "core/username.h"
#include "core/sock_id.h"

namespace tin {
class LoggedUser{
 public:
  enum Mode {
    NOTHING,
    GUEST,
    NORMAL,
    ADMIN,
  };

  constexpr LoggedUser() : sock_id_(0), mode_(Mode::NOTHING) {}
  explicit LoggedUser(const Username &, SockId, Mode);
  LoggedUser(const LoggedUser &) = delete;
  LoggedUser(LoggedUser &&);
  LoggedUser &operator=(LoggedUser &&);

  const Username &GetName() const {return name_;}
  int GetSockId() const {return sock_id_;}
  void SetSockId(int id) {sock_id_ = id;}
  void ClrSock() {sock_id_ = 0;}
  int ChMode(Mode mode) {
    if (mode_ == Mode::NOTHING || mode == Mode::NOTHING) {
      return -1;
    }
    mode_ = mode;
    return 0;
  }
  Mode GetMode() {
    return mode_;
  }

 private:
  void Move_(LoggedUser *);
  void Clear_();

  Username name_;
  SockId sock_id_;
  Mode mode_;
};  // class LoggedUser
}  // namespace tin

#endif  // SERVER_CORE_LOGGED_USER_H_
