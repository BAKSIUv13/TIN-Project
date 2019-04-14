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
#include "core/username.h"
#include "core/sock_stream_state.h"
#include "network/network_manager.h"
#include "network/nws.h"

namespace tin {
class Server {
 public:
  using LogUserTable = std::map<Username, LoggedUser>;
  using SessionToUserTable = std::map<SessionId, LoggedUser *>;
  using FdToUserTable = std::map<int, LoggedUser *>;
  using StatesTable = std::map<int, SockStreamState>;

  Server();
  Server(const Server &) = delete;
  ~Server();


  /// To nie wiem, czy będzie.
  void Run();
  void Run2();

  void SpecialHardcodeInit();

  int AddSession(SessionId, const Username &);
  void DelSession(SessionId);
  int RegisterSock(int fd);
  int DropSock(int fd);
  int AssocSessWithSock(SessionId, int fd);
  void DeassocSess(SessionId);
  void DeassocSock(int fd);

  void DealWithMsgs(int fd, const std::string &msg);
  void UploadFromState(int fd);

  /// From other thread?? This fn is blocking chyba.
  int StopRun2();

  void PrepareNws_();
  void UnprepareNws_();
  void DoAllTheThings_();

  World &GetWorld() {return world_;}
  // NetworkManager &GetNetManager() {return nm_;}
  TheConfig &GetConf() {return conf_;}
  AccountManager &GetAccountManager() {return am_;}

 private:
  World world_;
  LogUserTable users_;
  SessionToUserTable sess_to_users_;
  FdToUserTable fds_to_users_;
  StatesTable sss_;
  NetworkManager nm_;
  TheConfig conf_;
  AccountManager am_;
  Nws nws;
  int end_pipe_[2];
};  // class Server
}  // namespace tin

#endif  // SERVER_APP_SERVER_H_
