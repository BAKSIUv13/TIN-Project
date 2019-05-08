// Copyright 2019 piotrek

#ifndef SERVER_APP_SERVER_H_
#define SERVER_APP_SERVER_H_

#include <string>
#include <map>
#include <utility>
#include <vector>
#include <list>
#include <memory>

#include "account_manager/account_manager.h"
#include "app/logged_user.h"
#include "app/session.h"
#include "app/world.h"
#include "configurator/the_config.h"
#include "core/username.h"
#include "app/socket_stuff.h"
#include "core/socket_tcp4.h"
#include "core/sel.h"
#include "core/instr_id.h"
#include "app/instr_supp.h"

namespace tin {


class Server {
 public:
  static constexpr uint16_t DEFAULT_PORT = 42000;
  static constexpr bool DEAL_WITH_STDIN = true;
  static constexpr int DEFAULT_LISTEN_QUEUE_LEN = 32;
  static constexpr int NQS = sizeof(NQuad);

  static const std::map<InstrId, InstrSupp> instructions;

  Server();
  Server(const Server &) = delete;
  ~Server();

  // Function which start the server and returns when it finishes work.
  void Run(uint16_t port = DEFAULT_PORT,
    int queue_size = DEFAULT_LISTEN_QUEUE_LEN);

  // Nicee function which sets all necessary thigs in server xd
  void SpecialHardcodeInit();

  int AddSession(SessionId, const Username &);

  // Delete session and deassocs it if necessary.
  void DelSession(SessionId);
  int AssocSessWithSock(SessionId, int fd);

  // From other thread?? This fn is blocking chyba.
  int StopRun();

  // Returns username assigned to fd. Blank if fd does not have session.
  Username SockToUn(int fd);

  Username SidToUn(SessionId);

  // World &GetWorld() {return world_;}
  TheConfig &GetConf() {return conf_;}
  AccountManager &GetAccountManager() {return am_;}




 private:
  int PushMsg_(std::unique_ptr<OutMessage> msg);
  OutMessage *FirstMsg_();
  int PopMsg_();
  void DeassocSess_(SessionId);
  void DeassocSock_(int fd);

  // Reset state of server???
  int Reset_();

  // Initializes listenig socket with given parameters.
  int InitializeListener_(uint16_t port, int queue_size);

  // Function called in loop that do all the job that server has to.
  int LoopTick_();

  // Moves given socket to map of sockets in server class.
  int RegisterSockFromAccept_(SocketTCP4 &&);

  // Removes socket from the server.
  int DropSock_(int fd);

  // Sets bitset of servers's 'select' object.
  int FeedSel_();

  // Reads data from listening sock, standart input and closing pipe.
  int ReadMainFds_();

  // Calls 'select' function.
  int DoSel_();

  // Write data to client sockets.
  int WriteToSocks_();

  // Reads client sockets and deal with it.
  int ReadClients_();

  // Do some work with all received stuff.
  int DoWork_();

  // Reads one client socket.
  int ReadClientSocket_(int fd);

  // Deals with read data from client socket.
  int DealWithReadBuf_(int fd);

  // Ahenles stdin 'scripts'.
  int DealWithStdinBuf_(const char *);

  // Loads bytes expected as magic start word and checks if we have 'OwO!'.
  // RC - Read Client
  int RCMagic_(int fd, SocketTCP4 *sock, SocketStuff *stuff);

  // Loads instruction number.
  int RCInstrLd_(int fd, SocketTCP4 *sock, SocketStuff *stuff);

  // Loads second instruction number.
  int RCInstr2Ld_(int fd, SocketTCP4 *sock, SocketStuff *stuff);

  // Chooses and executes instruction.
  int RCExecInstr_(int fd, SocketTCP4 *sock, SocketStuff *stuff);

  int RCChooseFn_(int fd, SocketTCP4 *sock, SocketStuff *stuff);

  //// Tries to check if we have some number of bytes in buffer.
  // int RCLoadFirst_(int fd, SocketTCP4 *sock, SocStuff *stuff, int how_much);

  // Resets 'state of machine' that coś tam coś tam.
  int RCResetCm_(int fd, SocketTCP4 *sock, SocketStuff *stuff);

  // Deletes sockets marked to delete.
  int DeleteMarkedSocks_();

  // This variable tells if server is now running.
  bool runs_;

  World world_;
  Sel sel_;
  SocketTCP4 listening_sock_;
  std::map<int, std::pair<SocketTCP4, SocketStuff> > client_socks_;
  std::map<Username, LoggedUser> users_;
  std::map<SessionId, LoggedUser *> sess_to_users_;
  std::map<int, LoggedUser *> socks_to_users_;
  TheConfig conf_;
  AccountManager am_;
  std::list<std::unique_ptr<OutMessage> > messages_to_send_;

  // Pipe that may be used to tell the server to stop.
  int end_pipe_[2];
};  // class Server
}  // namespace tin

#endif  // SERVER_APP_SERVER_H_
