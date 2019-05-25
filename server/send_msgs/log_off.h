// Copyright 2019 Piotrek

#ifndef SERVER_SEND_MSGS_LOG_OFF_H_
#define SERVER_SEND_MSGS_LOG_OFF_H_

#include <string>

#include "core/out_message.h"
#include "core/write_buf.h"

namespace tin {

class LogOff : public OutMessage {
 public:
  LogOff() {}
  explicit LogOff(SockId id)
      : id_(id) {}

  virtual ~LogOff() {}

  virtual std::string GetTypeName() {return "LogOff";}

  virtual int Audience() {return ONE_S;}

  virtual SockId Sock() {return id_;}

  virtual int AddToBuf(WriteBuf *buf);
 private:
  SockId id_;
};  // class LogOff
}  // namespace tin

#endif  // SERVER_SEND_MSGS_LOG_OFF_H_
