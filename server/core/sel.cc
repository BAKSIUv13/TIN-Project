// Copyright 2019 Piotrek

#include "core/sel.h"

#include <unistd.h>
#include <sys/types.h>

#include <cstring>

// UWAGA!!! To może nie działać!!!

namespace tin {

static_assert(FD_SETSIZE == Sel::MAX_SIZE);

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

Sel::Sel()
    : buffer_len_(0) {
  memset(fds_, 0, sizeof(fds_));
}

Sel::~Sel() {}

void Sel::Zero() {
  buffer_len_ = 0;
  memset(fds_, 0, sizeof(fds_));
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
    set_bit(read_fds_(), fd);
  }
  if (t & WRITE) {
    set_bit(write_fds_(), fd);
  }
  if (t & EXCEPTION) {
    set_bit(exc_fds_(), fd);
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
    reset_bit(read_fds_(), fd);
  }
  if (t & WRITE) {
    reset_bit(write_fds_(), fd);
  }
  if (t & EXCEPTION) {
    reset_bit(exc_fds_(), fd);
  }
  Shrink_();
  return 0;
}

int Sel::Get(int fd) {
  if (fd >= buffer_len_) {
    return NONE;
  }
  int ret = NONE;
  if (get_bit(read_fds_(), fd)) {
    ret |= READ;
  }
  if (get_bit(write_fds_(), fd)) {
    ret |= WRITE;
  }
  if (get_bit(exc_fds_(), fd)) {
    ret |= EXCEPTION;
  }
  return ret;
}

void Sel::Shrink_() {
  int last = buffer_len_ - 1;
  while (last >= 0) {
    if (get_bit(read_fds_(), last)
      | get_bit(write_fds_(), last)
      | get_bit(exc_fds_(), last)) {
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
    reinterpret_cast<fd_set *>(read_fds_()),
    reinterpret_cast<fd_set *>(write_fds_()),
    reinterpret_cast<fd_set *>(exc_fds_()),
    tv_ptr);
  Shrink_();
  return ret;
}


}  // namespace tin
