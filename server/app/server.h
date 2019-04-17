// Copyright 2019 piotrek

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
#include "app/socket_stuff.h"
#include "core/socket_tcp4.h"
#include "core/sel.h"
#include "network/nws.h"

namespace tin {


class Server {
 public:
  // using LogUserTable = std::map<Username, LoggedUser>;
  // using SessionToUserTable = std::map<SessionId, LoggedUser *>;
  // using FdToUserTable = std::map<int, LoggedUser *>;
  // using StatesTable = std::map<int, SockStreamState>;


  static constexpr uint16_t DEFAULT_PORT = 42000;
  static constexpr bool DEAL_WITH_STDIN = true;
  static constexpr int DEFAULT_LISTEN_QUEUE_LEN = 32;
  static constexpr int BUF_SIZE = 256;


  Server();
  Server(const Server &) = delete;
  ~Server();

  void Run(uint16_t port = DEFAULT_PORT,
    int queue_size = DEFAULT_LISTEN_QUEUE_LEN);

  void SpecialHardcodeInit();

  int AddSession(SessionId, const Username &);
  void DelSession(SessionId);
  int AssocSessWithSock(SessionId, int fd);
  void DeassocSess(SessionId);
  void DeassocSock(int fd);

  // void DealWithMsgs(int fd, const std::string &msg);
  // void UploadFromState(int fd);

  /// From other thread?? This fn is blocking chyba.
  int StopRun();

  // void PrepareNws_();
  // void UnprepareNws_();
  // void DoAllTheThings_();

  World &GetWorld() {return world_;}
  // NetworkManager &GetNetManager() {return nm_;}
  TheConfig &GetConf() {return conf_;}
  AccountManager &GetAccountManager() {return am_;}

 private:
  int Reset_();
  int InitializeListener_(uint16_t port, int queue_size);
  int LoopTick_();
  int RegisterSockFromAccept_(SocketTCP4 &&);
  int DropSock_(int fd);
  int FeedSel_();
  int DoSel_();
  int DealWithSockets_();
  int DeleteMarkedSockets_();

  bool runs_;

  World world_;
  Sel sel_;
  SocketTCP4 listening_sock_;
  std::map<int, SocketTCP4> client_socks_;
  std::map<int, SocketStuff> sock_stuff_;
  std::map<Username, LoggedUser> users_;
  std::map<SessionId, LoggedUser *> sess_to_users_;
  std::map<int, LoggedUser *> socks_to_users_;
  // NetworkManager nm_;
  TheConfig conf_;
  AccountManager am_;
  // Nws nws;
  int end_pipe_[2];
};  // class Server
}  // namespace tin

#endif  // SERVER_APP_SERVER_H_
