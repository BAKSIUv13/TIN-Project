// Copyright 1029 Piotrek

#include "core/logger.h"
#include "app/socket_stuff.h"

#include "app/server.h"

namespace tin {

int SocketStuff::ChooseInstr() {
  // Zakładaamy, że już doczytaliśmy instrukcję.
  // Możliwe, że później to zmienięi8989i9i9i9i9i9i98uuiiiiiiiiiiiiiiiiiiiiiiiii
  InstrId instr_id(instr_);
  const InstrSupp *supp_ptr = serv_->GetInstr(instr_id);
  if (supp_ptr == nullptr) {
    LogM << "Nie ma instrukcji " << instr_ << " :< \n";
    supp_ = InstrSupp();
    return -1;
  }
  if (supp_ptr->Expands()) {
    int pom = ReadInstr2();
    if (pom < 0) {
      LogM << "Błąd przy doczytywaniu do wybieranka.\n";
      return pom;
    }
    if (pom > 0) {
      LogM << "nie doczytało \n";
      return 1;
    }
    instr_id = InstrId(instr_, instr2_);
    supp_ptr = serv_->GetInstr(instr_id);
    if (supp_ptr == nullptr) {
      LogM << "Nie ma instrukcji " << instr_<< ' '
        << instr2_ << " :< \n";
      supp_ = InstrSupp();
      return -1;
    }
  }
  if (!supp_ptr->GetFn()) {
    LogH << "Nie ma funkcji lel xd w instrukcji " << instr_
      << std::string(instr2_ ? std::string(instr2_) : std::string("--"))
      << "\n";
    supp_ = InstrSupp();
    return -1;
  } else {
    supp_ = *supp_ptr;
    strct_
      = reinterpret_cast<InstrStruct *>(malloc(supp_.GetSize()));
    ConstructFn fn = supp_.GetConstructor();
    if (fn) {
      fn(strct_);
    }
  }
  return 0;
}

}  // namespace tin
