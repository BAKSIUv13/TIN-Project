// Copyright 2077 jabka

#ifndef SERVER_CORE_THREAD_H_
#define SERVER_CORE_THREAD_H_

#include <pthread.h>

namespace tin {
class Thread {
 public:
  Thread();
  explicit Thread(void *(*)(void *), void *);
  Thread(const Thread &) = delete;
  Thread &operator=(const Thread &) = delete;
  Thread(Thread&&);
  Thread &operator=(Thread &&);
  ~Thread();
  bool Joinable();
  int Join(void **vp = nullptr);
  void join() {Join();}  // std
  int Cancel();
 private:
  void Destroy_();
  pthread_t thr_;
  pthread_attr_t attr_;
  void *(*function_)(void *);
  void *arg_;
  bool has_thread_;
  bool has_attr_;
};
}  // namespace tin

#endif  // SERVER_CORE_THREAD_H_
