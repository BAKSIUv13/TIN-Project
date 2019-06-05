// Copyright 2019 Piotrek

#ifndef SERVER_CORE_SERVER_H_
#define SERVER_CORE_SERVER_H_

#include <string>
#include <map>
#include <utility>
#include <vector>
#include <list>
#include <memory>

#include "account_manager/account_manager.h"
#include "core/logged_user.h"
#include "core/world.h"
#include "configurator/the_config.h"
#include "core/username.h"
#include "core/socket_stuff.h"
#include "core/socket_tcp4.h"
#include "core/sel.h"
#include "core/instr_id.h"
#include "core/instr_supp.h"
#include "core/sock_id.h"

namespace tin {

class Server {
 public:
  static constexpr uint16_t DEFAULT_PORT = 42000;
  static constexpr bool DEAL_WITH_STDIN = true;
  static constexpr int DEFAULT_LISTEN_QUEUE_LEN = 32;
  static constexpr int NQS = sizeof(NQuad);
  static constexpr int MAX_UN_LEN = Username::MAX_NAME_LEN;
  static constexpr int MAX_PW_LEN = 32;

  static const std::map<InstrId, InstrSupp> instructions;

  Server();
  Server(const Server &) = delete;
  ~Server();

  // Function which start the server and returns when it finishes work.
  void Run(uint16_t port = DEFAULT_PORT,
    int queue_size = DEFAULT_LISTEN_QUEUE_LEN);

  // Nicee function which sets all necessary thigs in server xd
  void SpecialHardcodeInit();

  // From other thread?? This fn is blocking chyba.
  int StopRun();

  // Returns username assigned to fd. Blank if fd does not have session.
  Username SockToUn(SockId id);

  // xd
  const InstrSupp *GetInstr(const InstrId &id) {
    if (instructions.count(id) < 1) {
      return nullptr;
    }
    return &instructions.at(id);
  }

  int LogInUser(const Username &un, const std::string &pw, SockId,
    bool generate_response);

  int LogOutUser(SockId, bool generate_response);

  template <typename T, typename... Args>
  T *PushMsg(Args &&... args) {
    T *x = new T(std::forward<Args>(args)...);
    std::unique_ptr<OutMessage> u(std::move(x));
    try {
      messages_to_send_.emplace_back(std::move(u));
    } catch (std::bad_alloc &e) {
      return nullptr;
    }
    return x;
  }


  class UserIterator {
   public:
    UserIterator() : server_(nullptr) {}
    UserIterator &operator++() {
      ++it_;
      return *this;
    }
    const Username *operator->() {
      return server_ ?
        &it_->first : nullptr;
    }
    const Username &operator*() {
      return *operator->();
    }
    bool operator!=(const UserIterator &other) const {
      return server_ != other.server_ || it_ != other.it_;
    }
   private:
    Server *server_;
    std::map<Username, LoggedUser>::const_iterator it_;
    friend class Server;
  };

  UserIterator usercbegin() {
    UserIterator it;
    it.server_ = this;
    it.it_ = users_.cbegin();
    return it;
  }

  UserIterator usercend() {
    UserIterator it;
    it.server_ = this;
    it.it_ = users_.cend();
    return it;
  }

 private:
  int PushMsg_(std::unique_ptr<OutMessage> msg);
  OutMessage *FirstMsg_();
  int PopMsg_();

  bool IsLogged_(const Username &un) {
    return users_.count(un) > 0;
  }

  // Reset state of server???
  int Reset_();

  // Initializes listenig socket with given parameters.
  int InitializeListener_(uint16_t port, int queue_size);

  // Function called in loop that do all the job that server has to.
  int LoopTick_();

  // Moves given socket to map of sockets in server class.
  int RegisterSockFromAccept_(SocketTCP4 &&);

  // Removes socket from the server.
  int DropSock_(SockId id);

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

  // Do work in world.
  int DoWorldWork_();

  // Write messages to buffers to send.
  int MsgsToBufs_();

  // Deals with read data from client socket.
  int DealWithReadBuf_(int fd);

  // Ahenles stdin 'scripts'.
  int DealWithStdinBuf_(const char *);

  // Deletes sockets marked to delete.
  int DeleteMarkedSocks_();

  int NextSockId_() {
    return next_sock_id_++;
  }

  // This variable tells if server is now running.
  bool runs_;

  World world_;
  Sel sel_;
  SocketTCP4 listening_sock_;
  std::map<SockId, SocketStuff> client_socks_;
  std::map<Username, LoggedUser> users_;
  std::map<SockId, LoggedUser *> socks_to_users_;
  TheConfig conf_;
  AccountManager am_;
  std::list<std::unique_ptr<OutMessage> > messages_to_send_;

  SockId next_sock_id_;

  // Pipe that may be used to tell the server to stop.
  int end_pipe_[2];
};  // class Server
}  // namespace tin

#endif  // SERVER_CORE_SERVER_H_
