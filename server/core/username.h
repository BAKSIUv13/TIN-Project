// Copyright 2019 Piotrek

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
  constexpr Username() : state_(BLANK), c_{'\0'} {}
  // Username();
  Username(const Username &);
  // Username(Username &&);
  Username &operator=(const Username &);
  // Username &operator=(Username &&);
  explicit Username(const std::string &);
  explicit Username(const char *);
  explicit Username(const Username &, CondenseFlag_t);


  int GetState() const {return state_;}

  bool operator==(const Username &o) const {return Compare_(o) == 0;}
  bool operator!=(const Username &o) const {return Compare_(o) != 0;}
  bool operator<(const Username &o) const {return Compare_(o) < 0;}
  bool operator>(const Username &o) const {return Compare_(o) > 0;}
  bool operator<=(const Username &o) const {return Compare_(o) <= 0;}
  bool operator>=(const Username &o) const {return Compare_(o) >= 0;}
  bool RawEqual(const Username &o) const {return RawCompare_(o) == 0;}
  bool RawLess(const Username &o) const {return RawCompare_(o) < 0;}

  operator const char *() const {return c_;}
  //  explicit operator std::string() const {return std::string(c_);}


 private:
  int state_;
  char c_[MAX_NAME_LEN + 1];

  int Check_() const;
  void ZeroBad_();
  int Compare_(const Username &) const;
  int RawCompare_(const Username &) const;
};

}  // namespace tin

#endif  // SERVER_CORE_USERNAME_H_
