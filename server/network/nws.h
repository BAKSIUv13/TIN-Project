// Copyright 2019 Piotrek

#ifndef SERVER_NETWORK_NWS_H_
#define SERVER_NETWORK_NWS_H_

// ntwork work space

// #include <tuple>
#include <vector>
#include <string>
#include  <map>

namespace tin {

struct Nws {
  struct Msg {
    int fd;
    std::string msg;
  };
  using Vec = std::vector<Msg>;
  using Map = std::map<int, std::string>;

  Vec received_messages;
  Map messages_to_send;

  std::vector<int> new_sockets;
  std::vector<int> sockets_to_drop;
  std::vector<int> sockets_to_close;

  void Clear();
  void AddMsgSend(int fd, const std::string &m);
  void AddSockToReg(int fd);
  void AddSockToDrop(int fd);
};

}  // namespace tin

#endif  // SERVER_NETWORK_NWS_H_
