// Copyright 2019 Piotrek

#include "core/sel.h"

#include <unistd.h>
#include <sys/types.h>

#include <cstring>

namespace tin {

/*
static void set_bit(void *where, int which) {
  uint8_t *buf = reinterpret_cast<uint8_t *>(where);
  buf[which / 8] |= 1 << (which % 8);
}

static void reset_bit(void *where, int which) {
  uint8_t *buf = reinterpret_cast<uint8_t *>(where);
  uint8_t q = -1, w = -1, e;
  q <<= which % 8 + 1;
  w >>= (8 - which % 8);
  e = q | w;
  buf[which / 8] &= e;
}

static bool get_bit(void *where, int which) {
  uint8_t *buf = reinterpret_cast<uint8_t *>(where);
  return buf[which / 8] & (1 << (which % 8));
}
*/

Sel::Sel()
    : buffer_len_(0) {
  FD_ZERO(&read_fds_);
  FD_ZERO(&write_fds_);
}

Sel::~Sel() {}

void Sel::Zero() {
  buffer_len_ = 0;
  FD_ZERO(&read_fds_);
  FD_ZERO(&write_fds_);
}

int Sel::AddFD(int fd, int t) {
  if ((t & ALL) == NONE) {
    return 0;
  }
  if (fd >= MAX_SIZE) {
    return -1;
  }
  if (fd + 1 > buffer_len_) {
    buffer_len_ = fd + 1;
  }
  if (t & READ) {
    FD_SET(fd, &read_fds_);
  }
  if (t & WRITE) {
    FD_SET(fd, &write_fds_);
  }
  return 0;
}

int Sel::DelFD(int fd, int t) {
  if ((t & ALL) == NONE) {
    return 0;
  }
  if (fd >= buffer_len_) {
    return 0;
  }
  if (fd >= MAX_SIZE) {
    return -1;
  }
  if (t & READ) {
    FD_CLR(fd, &read_fds_);
  }
  if (t & WRITE) {
    FD_CLR(fd, &write_fds_);
  }
  Shrink_();
  return 0;
}

int Sel::Get(int fd) {
  if (fd >= buffer_len_) {
    return NONE;
  }
  int ret = NONE;
  if (FD_ISSET(fd, &read_fds_)) {
    ret |= READ;
  }
  if (FD_ISSET(fd, &write_fds_)) {
    ret |= WRITE;
  }
  return ret;
}

void Sel::Shrink_() {
  int last = buffer_len_ - 1;
  while (last >= 0) {
    if (FD_ISSET(last, &read_fds_)
      | FD_ISSET(last, &write_fds_)) {
      break;
    }
    --last;
  }
  if (last < buffer_len_ - 1) {
    buffer_len_ = last + 1;
  }
}

int Sel::Select(TimeVal time_value) {
  struct timeval tv, *tv_ptr;
  if (time_value.infinite) {
    tv_ptr = nullptr;
  } else {
    tv_ptr = &tv;
    tv.tv_sec = time_value.seconds;
    tv.tv_usec = time_value.microseconds;
  }
  int ret = select(buffer_len_,
    &read_fds_,
    &write_fds_,
    nullptr,
    tv_ptr);
  Shrink_();
  return ret;
}

}  // namespace tin
