// Copyright 2077 jabka

#ifndef SERVER_APP_SERVER_H_
#define SERVER_APP_SERVER_H_

#include <string>
#include <map>

#include "app/logged_user.h"
#include "app/session.h"
#include "app/world.h"
#include "configurator/the_config.h"

namespace tin {
class Server {
 public:
  using LogUserTable = std::map<std::string, LoggedUser>;
  using SessionToUserTable = std::map<SessionId, LoggedUser *>;

  int Run();
 private:
  World world_;
  LogUserTable users_;
  SessionToUserTable users_by_sessions_;
  TheConfig conf_;
};  // class Server
}  // namespace tin

#endif  // SERVER_APP_SERVER_H_
