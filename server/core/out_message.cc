// Copyright 2019 Piotrek

#include "core/out_message.h"

namespace tin {

std::list<Username> OutMessage::Users() const {
  return std::list<Username>{};
}

std::list<SockId> OutMessage::Socks() const {
  return std::list<SockId>{};
}

Username OutMessage::User() const {
  return Username{};
}

SockId OutMessage::Sock() const {
  return 0;  // 0 nigdy nie jest używane
}

}  // namespace tin
