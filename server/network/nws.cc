// Copyright 2019 Piotrek

#include "network/nws.h"

#include <utility>
#include <iostream>

namespace tin {
void Nws::Clear() {
  received_messages.clear();
  messages_to_send.clear();
  new_sockets.clear();
  sockets_to_drop.clear();
  sockets_to_close.clear();
}
void Nws::AddMsgSend(int fd, const std::string &m) {
  if (messages_to_send.count(fd) > 0) {
    messages_to_send.at(fd).append(m);
  } else {
    messages_to_send.insert(std::make_pair(fd, m));
  }
}
void Nws::AddSockToReg(int fd) {
  new_sockets.push_back(fd);
}
void Nws::AddSockToDrop(int fd) {
  std::cerr << "Dodawanie socketu " << fd << " do usunięcia\n";
  sockets_to_drop.push_back(fd);
}





}  // namespace tin
