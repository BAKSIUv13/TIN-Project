// Copyright 21312 jeejfioed

#ifndef SERVER_NETWORK_NETWORK_MANAGER_H_
#define SERVER_NETWORK_NETWORK_MANAGER_H_

namespace tin {
class NetworkManager {
 public:
  NetworkManager();
  ~NetworkManager();

  void Start();
  void Stop();

  void Send();

  void GetThings();
};  // class NetworkManager
}  // namespace tin

#endif  // SERVER_NETWORK_NETWORK_MANAGER_H_
