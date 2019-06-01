// Copyright 2019 TIN

#ifndef SERVER_ACCOUNT_MANAGER_ACCOUNT_MANAGER_H_
#define SERVER_ACCOUNT_MANAGER_ACCOUNT_MANAGER_H_

#include <fstream>
#include <string>

namespace tin {
class AccountManager {
 public:
  AccountManager() {}
  ~AccountManager() {}

  void AttachFile() {}
  void DetachFile() {}

  void UserAdd(std::string name, std::string passwd, bool admin = false) {}
  void UserDel(std::string name) {}
  void UserChPass(std::string name, std::string passwd) {}
  void UserChPerm(std::string name, bool admin) {}

  bool Authenticate(std::string name, std::string pass) {return false;}

  /// Tells if user is an admin.
  bool Authorize(std::string name) {return false;}
 private:
  void ReadUser_() {}
  std::fstream the_file_;
};  // class AccountManager
}  // namespace tin

#endif  // SERVER_ACCOUNT_MANAGER_ACCOUNT_MANAGER_H_
