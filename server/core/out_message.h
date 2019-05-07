// Copyright 2019 Piotrek

#ifndef SERVER_CORE_OUT_MESSAGE_H_
#define SERVER_CORE_OUT_MESSAGE_H_

#include <string>
#include <list>

#include "core/username.h"
#include "core/write_buf.h"

namespace tin {

class OutMessage {
 public:
  OutMessage() {}
  virtual ~OutMessage() {}

  virtual std::string GetTypeName() = 0;

  // Send to all.
  virtual bool Broadcast() = 0;

  // Send to whom.
  virtual std::list<Username> Users() = 0;

  virtual int AddToBuf(WriteBuf *) = 0;
};
}  // namespace tin

#endif  // SERVER_CORE_OUT_MESSAGE_H_
