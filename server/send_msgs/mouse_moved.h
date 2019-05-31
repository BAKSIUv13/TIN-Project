// Copyright 2019 Piotrek

#ifndef SERVER_SEND_MSGS_MOUSE_MOVED_H_
#define SERVER_SEND_MSGS_MOUSE_MOVED_H_

#include <string>
#include <list>

#include "core/out_message.h"
#include "core/write_buf.h"

namespace tin {

// This is class of message that shall be broadcast.
class MouseMoved : public OutMessage {
 public:
  MouseMoved() {}
  MouseMoved(const Username &un, int32_t x, int32_t y)
      : username_(un), x_(x), y_(y) {}

  virtual ~MouseMoved() {}

  Username GetUsername() const {return username_;}
  constexpr int32_t x() const {return x_;}
  constexpr int32_t y() const {return y_;}

  virtual std::string GetTypeName() {return "MouseMoved";}
  virtual bool Broadcast() {return true;}
  virtual std::list<Username> Users() {
    return std::list<Username>{};
  }
  virtual int AddToBuf(WriteBuf *buf);
 private:
  Username username_;
  int32_t x_, y_;
};  // class MouseMoved
}  // namespace tin

#endif  // SERVER_SEND_MSGS_MOUSE_MOVED_H_
