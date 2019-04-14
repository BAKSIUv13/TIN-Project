// Copyright 21312 jeejfioed

#ifndef SERVER_NETWORK_NETWORK_MANAGER_H_
#define SERVER_NETWORK_NETWORK_MANAGER_H_

#include <vector>
#include <map>

#include "core/socket_tcp4.h"
#include "core/sel.h"
#include "app/session.h"

namespace tin {

struct NetStartConf {
  uint16_t port;
};

class NetworkManager {
 public:
  static constexpr int DEFAULT_QUEUE_LEN = 32;

  NetworkManager() {}
  ~NetworkManager() {}

  // void StartService() {}
  // void StopService() {}

  void Start(const NetStartConf &);

  // void Send() {}

  // void GetThings() {}

  void FeedMainSel(Sel *);
  void DealWithSelResult(Sel *, );
 private:
  SocketTCP4 sock;
  std::map<int, SocketTCP4> client_socks;
  std::map<SessionId, SocketTCP4 *> sess_to_socks;
  std::map<int, SessionId> fd_to_sess;
};  // class NetworkManager
}  // namespace tin

#endif  // SERVER_NETWORK_NETWORK_MANAGER_H_
