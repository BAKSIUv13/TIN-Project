// Copyright 2019 Piotrek

#include "core/out_message.h"

namespace tin {

std::list<Username> OutMessage::Users() {
  return std::list<Username>{};
}

std::list<SockId> OutMessage::Socks() {
  return std::list<SockId>{};
}

Username OutMessage::User() {
  return Username{};
}

SockId OutMessage::Sock() {
  return 0;  // 0 nigdy nie jest używane
}

}  // namespace tin
