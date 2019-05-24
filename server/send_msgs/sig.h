// Copyright 2019 Piotrek

#ifndef SERVER_SEND_MSGS_SIG_H_
#define SERVER_SEND_MSGS_SIG_H_

#include <string>
#include <list>

#include "core/out_message.h"
#include "core/write_buf.h"

namespace tin {

class Sig : public OutMessage {
 public:
  Sig() {}
  Sig(const Username &un, NQuad code, bool is_heavy,
    const std::string &msg)
      : username_(un), code_(code), msg_(msg), heavy_(is_heavy) {}
  Sig(const Username &un, NQuad code, bool is_heavy)
      : username_(un), code_(code), msg_(""), heavy_(is_heavy) {}

  virtual ~Sig() {}

  Username GetUsername() {return username_;}
  bool IsHeavy() {return heavy_;}
  const std::string GetMsg() {return msg_;}
  NQuad GetCode() {return code_;}

  virtual std::string GetTypeName() {
    return IsHeavy() ? "Signal Heavy" : "Signal Light";
  }
  virtual bool Broadcast() {return false;}
  virtual std::list<Username> Users() {
    return username_.Good() ?
      std::list<Username>{username_} : std::list<Username>{};
  }
  virtual int AddToBuf(WriteBuf *buf);

 private:
  Username username_;
  NQuad code_;
  std::string msg_;
  bool heavy_;
};  // class Sig
}  // namespace tin

#endif  // SERVER_SEND_MSGS_SIG_H_
