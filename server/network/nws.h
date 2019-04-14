// Copyright 0 0

#ifndef SERVER_NETWORK_NWS_H_
#define SERVER_NETWORK_NWS_H_

// ntwork work space

#include <tuple>
#include <vector>
#include <string>

namespace tin {

struct Nws {
  using Msg = std::tuple<int, std::string>;
  using Vec = std::vector<Msg>;

  Vec received_messages;
  Vec messages_to_send;

  void Clear();
};

}  // namespace tin

#endif  // SERVER_NETWORK_NWS_H_
