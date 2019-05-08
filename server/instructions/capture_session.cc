// Copyright 2019 Piotrek

#include <iostream>
#include <memory>
#include <utility>

#include "instructions/capture_session.h"

#include "send_msgs/sess_capt_ok.h"

namespace tin {
/*int CaptureSession::Fn(Server *server, int fd, SocketTCP4 *sock,
    SocketStuff *stuff) {
  std::cerr << "Hehe mam wczytkanko sesji już prawie.\n";
  return 0;
}*/



int CaptureSession::Fn(Server *server, int fd,
    SocketStuff *stuff, World *, MsgPushFn push_fn) {
  CaptureSession *s = reinterpret_cast<CaptureSession *>(stuff->strct);
  int chars_to_copy = stuff->CountCopy(END);
  stuff->Copy(&s->sess_id_[stuff->cm_processed - START], chars_to_copy);
  if (stuff->cm_processed < END) {
    return 1;
  }
  std::cerr << "zajmowanko sesji " << s->GetSid_() << "\n";
  int pom = server->AssocSessWithSock(s->GetSid_(), fd);
  if (pom < 0) {
    std::cerr << "hehe xd\n";
    return -1;
  }
  std::cerr << "zajęło xd\n";
  Username un = server->SidToUn(s->GetSid_());
  if (!un) {
    std::cerr << "Heh, mam potwierdzić, że zalogował się ktoś, kogo nie ma, ok "
      << "xd\n";
      return -1;
  }
  std::unique_ptr<OutMessage> msg {new SessCaptOk(un)};
  (server->*push_fn)(std::move(msg));
  return 0;
}

void CaptureSession::Construct(InstrStruct *q) {
  new(q) CaptureSession();
}

void CaptureSession::Destroy(InstrStruct *q) {
  // reinterpret_cast<CaptureSession *>(q)->~CaptureSession();
  q->~InstrStruct();
  std::cerr << "Capture  : destroyv " << q << '\n';
}


}  // namespace tin
