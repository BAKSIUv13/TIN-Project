// Copyright 2019 TIN

#ifndef SERVER_ACCOUNT_MANAGER_ACCOUNT_MANAGER_H_
#define SERVER_ACCOUNT_MANAGER_ACCOUNT_MANAGER_H_

#include <fstream>
#include <string>

#include "core/username.h"

namespace tin {
class AccountManager {
 public:
  enum UserType {
    NOT_ALLOWED,
    GUEST,
    NORMAL,
    ADMIN,
  };
  enum State {
    BLANK,
    ATTACHED_RD,
    ATTACHED_RDWR,
  };
  static constexpr size_t MAX_SALT_LEN = 32;

  AccountManager() : state_(BLANK) {}
  ~AccountManager() {}

  int AttachFile(const char *, bool writable);
  void DetachFile() {}

  UserType GetUserInfo(const Username &);

  void UserAdd(Username, std::string passwd) {}
  void UserDel(Username) {}
  void UserChPass(Username, std::string passwd) {}
  void UserChPerm(Username, bool admin) {}

  int Authenticate(Username *, std::string pass);

  /// Tells if user is an admin.
  bool Authorize(const Username &) {return false;}

 private:
  void ReadUser_() {}

  std::fstream the_file_;
  State state_;
};  // class AccountManager
}  // namespace tin

#endif  // SERVER_ACCOUNT_MANAGER_ACCOUNT_MANAGER_H_
