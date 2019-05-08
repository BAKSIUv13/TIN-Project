// Copyright 2019 Piotrek

#ifndef SERVER_CORE_SEL_H_
#define SERVER_CORE_SEL_H_

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
    EXCEPTION = 1 << 2,
    ALL = READ | WRITE | EXCEPTION,
  };
  static constexpr int MAX_SIZE = 1024;
  // const, nie constexpr, bo nie chce include'ować
  // tych makr wszystkich tutaj

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
  using mask_int_t = uint8_t;
  mask_int_t fds_[3 * MAX_SIZE / sizeof(mask_int_t)];
  mask_int_t *read_fds_() {return fds_;}
  mask_int_t *write_fds_() {return fds_ + MAX_SIZE / sizeof(mask_int_t);}
  mask_int_t *exc_fds_() {return fds_ + 2 * MAX_SIZE / sizeof(mask_int_t);}
};  // class Sel

}  // namespace tin

#endif  // SERVER_CORE_SEL_H_
