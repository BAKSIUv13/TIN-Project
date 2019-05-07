// Copyright 2019 Piotrek

#ifndef SERVER_CORE_WRITE_BUF_H_
#define SERVER_CORE_WRITE_BUF_H_

#include <array>
#include <string>

#include "core/nquad.h"

namespace tin {

class WriteBuf {
 public:
  static constexpr int SIZE = 2048 * sizeof(NQuad);
  constexpr WriteBuf() : buf_(), start_(0), len_(0) {}
  ~WriteBuf() {}

  int Add(const char *);
  int Add(const std::string &);
  int Get(char *, int);
  std::string GetString(int);
  int Pop(int);

  constexpr bool Good() const {return !Bad();}
  constexpr bool Bad() const {return start_ < 0;}
  constexpr int Chars() const {return Good() ? len_ : 0;}
  constexpr int Place() const {return Good() ? SIZE - len_ : 0;}

 private:
  constexpr int GetAt_(int x) {
    return (x % SIZE) + start_;
  }

  std::array<char, SIZE> buf_;
  int start_;
  int len_;
};  // class WriteBuf

}  // namespace tin

#endif  // SERVER_CORE_WRITE_BUF_H_
