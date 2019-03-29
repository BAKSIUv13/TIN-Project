// Copyright 1410 Jurand ze Spychowa

#include "core/thread.h"

#include <cstring>
#include <exception>

namespace tin {
  Thread::Thread()
      : has_thread_(false), has_attr_(false) {
  }
  Thread::Thread(void *(*function)(void *), void *arg)
      : function_(function), arg_(arg), has_thread_(true), has_attr_(true) {
    pthread_attr_init(&attr_);
    pthread_create(&thr_, &attr_, function, arg);
  }
  Thread::Thread(Thread &&other) {
    std::memcpy(this, &other, sizeof(*this));
    other.has_thread_ = false;
  }
  Thread &Thread::operator=(Thread &&other) {
    Destroy_();
    std::memcpy(this, &other, sizeof(*this));
    other.has_thread_ = false;
    return *this;
  }
  Thread::~Thread() {
    Destroy_();
  }
  bool Thread::Joinable() {
    return has_thread_;  // Tutaj nie jestem pewnym, czy to jest dobrze.
  }
  int Thread::Join(void **vp) {
    int join_ret;
    join_ret = pthread_join(thr_, vp);
    has_thread_ = false;
    return join_ret;
  }
  int Thread::Cancel() {
    int cancel_ret;
    cancel_ret = pthread_cancel(thr_);
    has_thread_ = false;  // Tutaj może być źle xd
    return cancel_ret;
  }
  void Thread::Destroy_() {
    if (Joinable()) {
      std::terminate();
    }
    if (has_attr_) {
      pthread_attr_destroy(&attr_);
    }
  }

}  // namespace tin
