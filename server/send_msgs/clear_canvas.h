// Copyright 2019 Piotrek

#ifndef SERVER_SEND_MSGS_CLEAR_CANVAS_H_
#define SERVER_SEND_MSGS_CLEAR_CANVAS_H_

#include <string>
#include <list>

#include "core/out_message.h"
#include "core/write_buf.h"

namespace tin {

class ClearCanvas : public OutMessage {
 public:
  ClearCanvas() {}
  explicit ClearCanvas(SockId id) {}

  virtual ~ClearCanvas() {}

  virtual std::string GetTypeName() const {return "ClearCanvas";}

  virtual int Audience() const {return BROADCAST_U;}

  virtual std::string GetStr() const;
};  // class LogOk
}  // namespace tin

#endif  // SERVER_SEND_MSGS_CLEAR_CANVAS_H_
