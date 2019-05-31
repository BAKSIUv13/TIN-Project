// Copyright 2019 Piotrek

#include <iostream>
#include <utility>
#include <memory>

#include "instructions/say.h"

#include "send_msgs/user_msg.h"

namespace tin {
int Say::Fn(Server *server, SocketStuff *stuff, World *world,
    MsgPushFn push_fn) {
  /*
  Say *s = reinterpret_cast<Say *>(stuff->strct);
  int chars_to_copy;
  if (stuff->cm_processed < LEN_END) {
    chars_to_copy = stuff->CountCopy(LEN_END);
    stuff->Copy(&s->len_buf_[stuff->cm_processed - START], chars_to_copy);
    if (stuff->cm_processed < LEN_END) {
      return 1;
    }
  }
  if (!s->len_is_read_) {
    s->len_is_read_ = true;
    std::cerr << "Ok, mam wiadomość o długości " << s->Len_() << "\n";
    s->un_ = server->SockToUn(fd);
    if (s->un_.Good()) {
      std::cerr << "Od użytkownika [[" << s->un_ << "]]\n";
    } else {
      std::cerr << "Gniazdo " << fd << " nie jest zalogowane, więc nie może "
        "wysyłać wiadomości.\n";
      return -1;
    }
  }
  // stuff->cm_processed >= LEN_END
  chars_to_copy = stuff->CountCopy(LEN_END + s->Len_());
  stuff->CopyToCpp11String(&s->message_, chars_to_copy);
  if (stuff->cm_processed < LEN_END + s->Len_()) {
    return 1;
  }
  std::cerr << "Ok, ogarnianie wiadomości skończone, więc możemy ją dodać.\n";
  std::unique_ptr<OutMessage> msg {new UserMsg(s->un_, s->message_)};
  (server->*push_fn)(std::move(msg));
  */
  return 0;
}

void Say::Construct(InstrStruct *q) {
  new(q) Say();
}

void Say::Destroy(InstrStruct *q) {
  // reinterpret_cast<CaptureSession *>(q)->~CaptureSession();
  q->~InstrStruct();
  std::cerr << "say  : destroyv " << q << '\n';
}

}  // namespace tin
