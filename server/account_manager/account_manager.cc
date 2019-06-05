// Copyright 2019 TIN

#include "account_manager/account_manager.h"

#include <unistd.h>

#include <set>
#include <vector>
#include <iostream>
#include <cstring>
#include <ios>
#include <utility>


namespace tin {
  int AccountManager::AttachFile(const char *path, bool writable) {
    if (state_ != BLANK) return -1;
    the_file_.open(path, writable ?
      std::ios::in | std::ios::out :
      std::ios::in);
    if (!the_file_.is_open()) {
      return -1;
    }
    state_ = writable ? ATTACHED_RDWR : ATTACHED_RD;
    return 0;
  }
}  // namespace tin
