// Copyright 2019 Piotrek

#ifndef SERVER_SEND_MSGS_LOG_OK_H_
#define SERVER_SEND_MSGS_LOG_OK_H_

#include <string>
#include <list>

#include "core/out_message.h"
#include "core/write_buf.h"

namespace tin {

class LogOk : public OutMessage {
 public:
  LogOk() {}
  explicit LogOk(SockId id)
      : id_(id) {}

  virtual ~LogOk() {}

  virtual std::string GetTypeName() const {return "LogOk";}

  virtual int Audience() const {return ONE_S;}

  virtual SockId Sock() const {return id_;}

  virtual std::string GetStr() const;
 private:
  SockId id_;
};  // class LogOk
}  // namespace tin

#endif  // SERVER_SEND_MSGS_LOG_OK_H_
