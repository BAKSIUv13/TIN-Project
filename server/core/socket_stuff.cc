// Copyright 2019 TIN

#include "core/logger.h"
#include "core/socket_stuff.h"

#include "core/server.h"

namespace tin {

int SocketStuff::ChooseInstr() {
  // Zakładaamy, że już doczytaliśmy instrukcję.
  // Możliwe, że później to zmienię
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
  supp_ = *supp_ptr;
  strct_ = supp_.Create();
  assert(strct_);
  return 0;
}

}  // namespace tin
