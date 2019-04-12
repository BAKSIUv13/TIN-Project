// Copyright 21312 jeejfioed

#ifndef SERVER_NETWORK_NETWORK_MANAGER_H_
#define SERVER_NETWORK_NETWORK_MANAGER_H_

#include <vector>

namespace tin {
class NetworkManager {
 public:
  NetworkManager() {}
  ~NetworkManager() {}

  void StartService() {}
  void StopService() {}

  void Send() {}

  void GetThings() {}
 private:
  int tcp4_sock_;
  int tcp6_sock_;
  std::vector<int> clients_;
};  // class NetworkManager
}  // namespace tin

#endif  // SERVER_NETWORK_NETWORK_MANAGER_H_
