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
  MouseMoved(const Username &un, double x, double y)
      : username_(un), x_(x), y_(y) {}

  virtual ~MouseMoved() {}

  virtual std::string GetTypeName() const {return "MouseMoved";}

  virtual int Audience() const {return BROADCAST_U;}

  Username GetUsername() const {return username_;}
  constexpr double x() const {return x_;}
  constexpr double y() const {return y_;}

  virtual std::string GetStr() const;
 private:
  Username username_;
  double x_, y_;
};  // class MouseMoved
}  // namespace tin

#endif  // SERVER_SEND_MSGS_MOUSE_MOVED_H_
