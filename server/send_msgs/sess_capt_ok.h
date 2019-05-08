// Copyright 2019 Piotrek

#ifndef SERVER_SEND_MSGS_SESS_CAPT_OK_H_
#define SERVER_SEND_MSGS_SESS_CAPT_OK_H_

#include <string>
#include <list>

#include "core/out_message.h"
#include "core/write_buf.h"

namespace tin {

class SessCaptOk : public OutMessage {
 public:
  SessCaptOk() {}
  explicit SessCaptOk(const Username &un)
      : username_(un) {}

  virtual ~SessCaptOk() {}

  Username GetUsername() {return username_;}

  virtual std::string GetTypeName() {return "SessCaptOk";}
  virtual bool Broadcast() {return false;}
  virtual std::list<Username> Users() {
    return username_.Good() ?
      std::list<Username>{username_} : std::list<Username>{};
  }
  virtual int AddToBuf(WriteBuf *buf);
 private:
  Username username_;
};  // class UserMsg
}  // namespace tin

#endif  // SERVER_SEND_MSGS_SESS_CAPT_OK_H_
