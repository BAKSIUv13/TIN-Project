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
  Sig(SockId id, NQuad code, bool is_heavy,
    const std::string &msg)
      : id_(id), code_(code), msg_(msg), heavy_(is_heavy) {}
  Sig(SockId id, NQuad code, bool is_heavy)
      : id_(id), code_(code), msg_(""), heavy_(is_heavy) {}

  virtual ~Sig() {}

  virtual std::string GetTypeName() const {
    return IsHeavy() ? "Signal Heavy" : "Signal Light";
  }

  virtual int Audience() const {return ONE_S;}
  virtual SockId Sock() const {return id_;}
  virtual int AddToBuf(WriteBuf *buf) const;

  bool IsHeavy() const {return heavy_;}
  const std::string &GetMsg() const {return msg_;}
  NQuad GetCode() const {return code_;}

 private:
  SockId id_;
  NQuad code_;
  std::string msg_;
  bool heavy_;
};  // class Sig
}  // namespace tin

#endif  // SERVER_SEND_MSGS_SIG_H_
