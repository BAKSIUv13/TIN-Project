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
    GUEST,
    NORMAL,
    ADMIN,
  };

  AccountManager() {}
  ~AccountManager() {}

  void AttachFile(const char *) {}
  void DetachFile() {}

  UserType GetUserInfo(const Username &);

  void UserAdd(Username, std::string passwd) {}
  void UserDel(Username) {}
  void UserChPass(Username, std::string passwd) {}
  void UserChPerm(Username, bool admin) {}

  bool Authenticate(const Username &, std::string pass) {return false;}

  /// Tells if user is an admin.
  bool Authorize(const Username &) {return false;}

 private:
  void ReadUser_() {}
  std::fstream the_file_;
};  // class AccountManager
}  // namespace tin

#endif  // SERVER_ACCOUNT_MANAGER_ACCOUNT_MANAGER_H_
