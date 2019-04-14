// Copyright 0 0

#include "network/nws.h"

namespace tin {
  void Nws::Clear() {
    received_messages.clear();
    messages_to_send.clear();
  }
}  // namespace tin
