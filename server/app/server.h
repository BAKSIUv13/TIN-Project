// Copyright 2077 jabka

#ifndef SERVER_APP_SERVER_H_
#define SERVER_APP_SERVER_H_

#include <string>
#include <map>

#include "account_manager/account_manager.h"
#include "app/logged_user.h"
#include "app/session.h"
#include "app/world.h"
#include "configurator/the_config.h"
#include "network/network_manager.h"

namespace tin {
class Server {
 public:
  using LogUserTable = std::map<std::string, LoggedUser>;
  using SessionToUserTable = std::map<SessionId, LoggedUser *>;

  Server();
  Server(const Server &) = delete;


  /// To nie wiem, czy będzie.
  void Run();



  World &GetWorld() {return world_;}
  NetworkManager &GetNetManager() {return nm_;}
  TheConfig &GetConf() {return conf_;}
  AccountManager &GetAccountManager() {return am_;}

 private:
  World world_;
  LogUserTable users_;
  SessionToUserTable users_by_sessions_;
  NetworkManager nm_;
  TheConfig conf_;
  AccountManager am_;
};  // class Server
}  // namespace tin

#endif  // SERVER_APP_SERVER_H_
