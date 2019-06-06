// Copyright 2019 TIN

#ifndef SERVER_ACCOUNT_MANAGER_ACCOUNT_MANAGER_H_
#define SERVER_ACCOUNT_MANAGER_ACCOUNT_MANAGER_H_

#include <fstream>
#include <string>
#include <random>

#include "core/username.h"

namespace tin {
class AccountManager {
 public:
  enum UserPass {
    NOT_ALLOWED = 0,
    GUEST,
    NORMAL,
    ADMIN,
  };
  enum State {
    BLANK,
    ATTACHED_RD,
    ATTACHED_RDWR,
  };
  enum class GuestAccess {
    NONE,
    EMPTY_PASSWD,
    ANY_PASSWD,
  };

  using Rand = std::mt19937_64;

  static constexpr size_t MAX_SALT_LEN = 32;
  static constexpr Rand::result_type INIT_SEED = 32390;

  AccountManager() : state_(BLANK), ga_(GuestAccess::ANY_PASSWD),
      the_file_(nullptr) {
    rand_.seed(INIT_SEED ^ rand_.default_seed);
  }
  ~AccountManager() {
    if (the_file_) {
      fclose(the_file_);
      the_file_ = nullptr;
    }
  }

  int AttachFile(const char *, bool writable);
  int DetachFile();

  UserPass GetUserInfo(const Username &);

  int UserAdd(const Username &, std::string passwd, bool admin);
  int UserDel(const Username &);
  int UserChPass(const Username &, const std::string &passwd);
  int UserChRole(const Username &, bool admin);

  int Authenticate(Username *, std::string passwd);

  /// Tells if user is an admin.
  // bool Authorize(const Username &) {return false;}

  constexpr bool On() {
    return state_ == State::ATTACHED_RD || state_ == State::ATTACHED_RDWR;
  }

  constexpr bool Writable() {
    return state_ == State::ATTACHED_RDWR;
  }

  void FeedRand(Rand::result_type number) {
    rand_.seed(rand_() ^ number);
  }

  static int test(int argc, char **argv, char **env);

 private:

  State state_;
  GuestAccess ga_;
  FILE *the_file_;
  Rand rand_;
};  // class AccountManager
}  // namespace tin

#endif  // SERVER_ACCOUNT_MANAGER_ACCOUNT_MANAGER_H_
