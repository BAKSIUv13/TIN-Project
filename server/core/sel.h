// Copyright 2019 TIN

#ifndef SERVER_CORE_SEL_H_
#define SERVER_CORE_SEL_H_

#include <sys/select.h>

#include <vector>
#include <cstdint>

namespace tin {

struct TimeVal {
  int64_t seconds;
  int64_t microseconds;
  bool infinite;
};

class Sel {
 public:
  enum Type {
    NONE = 0,
    READ = 1,
    WRITE = 1 << 1,
    EXCEPTION = 1 << 2,  // EXCEPTION nie działa
    ALL = READ | WRITE | EXCEPTION,
  };
  static constexpr int MAX_SIZE = FD_SETSIZE;

  Sel();
  ~Sel();

  int Select(TimeVal);
  int Select() {
    return Select(TimeVal{0, 0, true});
  }

  int AddFD(int fd, int t);
  int DelFD(int fd, int t);
  int Get(int fd);
  void Zero();

 private:
  void Shrink_();
  int buffer_len_;
  fd_set read_fds_;
  fd_set write_fds_;
};  // class Sel

}  // namespace tin

#endif  // SERVER_CORE_SEL_H_
