// Copyright 2077 jabka

#ifndef SERVER_APP_SERVER_RUN_H_
#define SERVER_APP_SERVER_RUN_H_

#include "app/server.h"

namespace tin {
class ServerRun {
 public:
  ServerRun() = delete;
  explicit ServerRun(Server *);
  ~ServerRun();

  int Run();
 private:
  Server *server_;
};  // class ServerRun
}  // namespace tin

#endif  // SERVER_APP_SERVER_RUN_H_
