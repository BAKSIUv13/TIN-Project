// Copyright 0 0

#ifndef SERVER_CORE_USERNAME_H_
#define SERVER_CORE_USERNAME_H_

#include <string>

namespace tin {

class Username {
 public:
  enum : int {
    BLANK = 0,
    GOOD = 1,
    CONDENSED = 3,
    BAD = 4,
  };
  static constexpr struct CondenseFlag_t {} CondenseFlag {};
  static constexpr int MAX_NAME_LEN = 16;
  Username();
  Username(const Username &);
  // Username(Username &&);
  Username &operator=(const Username &);
  // Username &operator=(Username &&);
  explicit Username(const std::string &);
  explicit Username(const char *);
  explicit Username(const Username &, CondenseFlag_t);


  int GetState() const {return state_;}

  bool operator==(const Username &) const;
  bool operator!=(const Username &) const;

  operator std::string() {return std::string(c_);}
  operator const char *() {return c_;}


 private:
  int state_;
  char c_[MAX_NAME_LEN + 1];

  int Check_() const;
  bool RawEqual_(const Username &) const;
};

}  // namespace tin

#endif  // SERVER_CORE_USERNAME_H_
