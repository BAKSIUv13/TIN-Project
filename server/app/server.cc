// Copyright 2077 piotrek

#include "app/server.h"

#include <unistd.h>

#include <set>
#include <vector>
#include <iostream>
#include <cstring>
#include <ios>
#include <utility>

#include "core/socket_tcp4.h"
#include "core/nquad.h"
#include "core/mquads.h"

#include "instructions/capture_session.h"

static constexpr int STDIN_FD = STDIN_FILENO;

namespace {

class TempIO {
 public:
  explicit TempIO(std::ios *ios)
    : ios_(ios), ios_state_(nullptr) {
    ios_state_.copyfmt(*ios);
  }
  ~TempIO() {
    ios_->copyfmt(ios_state_);
  }
 private:
  std::ios *ios_;
  std::ios ios_state_;
};
}

namespace tin {
Server::Server()
    : runs_(false) {
  if (pipe(end_pipe_)) {
    std::cerr << "Serwer się nie zrobił bo potok się nie otworzył :< "
      << std::strerror(errno) << '\n';
    std::terminate();
  }
  return;
}
Server::~Server() {
  close(end_pipe_[0]);
  close(end_pipe_[1]);
}

const std::map<InstrId, InstrSupp> Server::instructions {
  {InstrId(MQ::CAPTURE_SESSION),
    InstrSupp(
      &CaptureSession::Fn,
      sizeof(CaptureSession),
      &CaptureSession::Construct,
      &CaptureSession::Destroy)},

  {InstrId(MQ::REQUEST_LOGIN),
    InstrSupp(
      nullptr,
      0,
      nullptr,
      nullptr)},

  {InstrId(MQ::SEND_MESSAGE),
    InstrSupp(InstrSupp::EXPAND)}
};

int Server::InitializeListener_(uint16_t port, int queue_size) {
  int result = 0;
  result = listening_sock_.Open();
  if (result < 0) return -1;
  result = listening_sock_.BindAny(port);
  if (result < 0) return -2;
  result = listening_sock_.Listen(queue_size);
  if (result < 0) return -3;
  std::cerr << "Numer gniazda słuchającego: " << listening_sock_.GetFD()
    << '\n';
  return 0;
}

void Server::Run(uint16_t port, int queue_size) {
  int init_ret = InitializeListener_(port, queue_size);
  if (init_ret < 0) {
    return;
  }
  bool not_exit = true;
  runs_ = true;
  while (not_exit && runs_) {
    LoopTick_();
    continue;
  }
}


int Server::FeedSel_() {
  sel_.Zero();
  if (DEAL_WITH_STDIN) {
    sel_.AddFD(STDIN_FD, Sel::READ);
  }
  sel_.AddFD(end_pipe_[0], Sel::READ);
  sel_.AddFD(listening_sock_.GetFD(), Sel::READ);
  for (auto sock_it = client_socks_.begin();
      sock_it != client_socks_.end();
      ++sock_it) {
    if (sock_it->second.second.shall_read) {
      sel_.AddFD(sock_it->first, Sel::READ);
    }
    if (0 && sock_it->second.second.shall_write) {
      sel_.AddFD(sock_it->first, Sel::WRITE);
    }
  }
  return 0;
}

int Server::RegisterSockFromAccept_(SocketTCP4 &&sock) {
  int fd = sock.GetFD();
  auto emplace_ret = client_socks_.emplace(fd,
    std::make_pair(std::move(sock), SocketStuff {}));
  if (!emplace_ret.second) {
    std::cerr << "Nie udało się dodać socketu do mapy z socketami :< " << fd
      << "trzeba coś z tym zrobić.\n";
    return -1;
  }
  emplace_ret.first->second.second.shall_read = true;
  return 0;
}

int Server::DoSel_() {
  return sel_.Select();
}

int Server::ReadMainFds_() {
  static constexpr std::streamsize IN_BUF_SIZE = 256;
  char in_buf[IN_BUF_SIZE];
  if (Sel::READ & sel_.Get(end_pipe_[0])) {
    std::cerr << "Przyszło zamknięcie ze specjanego potoku.\n";
    runs_ = false;
    return 1;
  }
  if (DEAL_WITH_STDIN && (Sel::READ & sel_.Get(STDIN_FD))) {
    std::cin.getline(in_buf, IN_BUF_SIZE);
    if (std::cin.eof()) {
      std::cerr << "Na stdin przyszedł koniec, zamykanko :>\n";
      runs_ = false;
    } else if (std::cin.good()) {
      std::cerr << "Na stdin wpisano:\n" << in_buf << '\n';
    } else if (std::cin.bad()) {
      std::cerr << "Przyszło coś na stdin, ale coś było nie tak, jak trzeba, "
        << " pomijam.\n";
    }
    std::cin.clear();
    return 1;
  }
  if (Sel::READ & sel_.Get(listening_sock_.GetFD())) {
    std::cerr << "Nowe połącznoko :>\n";
    SocketTCP4 sock;
    int acc_ret = listening_sock_.Accept(&sock);
    if (acc_ret < 0) {
      std::cerr << "Jakiś błąd przy accepcie :<\n";
    } else {
      RegisterSockFromAccept_(std::move(sock));
    }
  }
  return 0;
}

int Server::StopRun() {
  return write(end_pipe_[1], "", 1);
}

int Server::WriteToSocks_() {
  return 0;
}


int Server::ReadClients_() {
  int sockets_read = 0;
  for (auto it = client_socks_.begin(); it != client_socks_.end(); ++it) {
    if (!(it->second.second.shall_read)
        || !(Sel::READ & sel_.Get(it->first))) {
      continue;
    }
    if (ReadClientSocket_(it->first) >= 0) {
      ++sockets_read;
    }
  }
  return sockets_read;
}

int Server::ReadClientSocket_(int fd) {
  // SocketTCP4 &sock = client_socks_.at(fd).first;
  SocketStuff &stuff = client_socks_.at(fd).second;

  ssize_t read_ret = read(fd, stuff.read_buf, stuff.BUF_SIZE);
  if (read_ret < 0) {
    std::cerr << "Błąd przy czytaniu socketu " << fd
      << "\n errno: " << std::strerror(errno) << "\n";
    return -1;
  } else if (read_ret == 0) {
    std::cerr << "Na gniazdo przyszło zamknięcie.\n";
    stuff.marked_to_delete = true;
    // ZAMKNIĘCIE
    return 1;
  }
  stuff.read_buf[read_ret] = '\0';
  stuff.read_len = read_ret;
  stuff.read_processed = 0;
  std::cerr << "Na socket o numerze " << fd << " przyszło:\n"
    << stuff.read_buf << "\n";
  int deal_ret = DealWithReadBuf_(fd);
  if (deal_ret < 0) {
    std::cerr << "Był jakiś błąd przy ogarnianiu tego co przyszło z gniazda "
      << fd << ", zamykanko go przy następnym tym tym.\n";
    stuff.marked_to_delete = true;
    return -1;
  }
  return 0;
}

// int Server::RCLoadTo_(int fd, SocketTCP4 *sock, SocketStuff *stuff,
//    int how_much) {
//  int/
// }

int Server::RCResetCm_(int fd, SocketTCP4 *sock, SocketStuff *stuff) {
  stuff->cm_processed = 0;
  if (stuff->supp && stuff->supp.GetFn()) {
    DestroyFn fn = stuff->supp.GetDestructor();
    if (fn) {
      fn(stuff->strct);
    }
    free(stuff->strct);
    stuff->strct = nullptr;
    stuff->supp = InstrSupp();
  }
  return 0;
}

int Server::RCMagic_(int fd, SocketTCP4 *sock, SocketStuff *stuff) {
  int chars_to_copy = stuff->read_len - stuff->read_processed;
  if (chars_to_copy > NQS - stuff->cm_processed) {
    chars_to_copy = NQS - stuff->cm_processed;
  }
  memcpy(&stuff->first_quads[stuff->cm_processed],
    &stuff->read_buf[stuff->read_processed],
    chars_to_copy);
  stuff->read_processed += chars_to_copy;
  stuff->cm_processed += chars_to_copy;
  if (stuff->cm_processed < NQS)
    return 1;  // 1 means that we didn't reach NQS yet
  if (stuff->magic() != MQ::OWO) {
    std::cerr << "Nie 'OwO!' :<\n";
    // stuff->errors.push(stuff->NOT_OWO);
    return - 1 - 4096;  // Niech będzie że to na razie prawie przypadkowa
                        // liczba.
  }
  return 0;
}

int Server::RCInstrLd_(int fd, SocketTCP4 *sock, SocketStuff *stuff) {
  int chars_to_copy = stuff->read_len - stuff->read_processed;
  if (chars_to_copy > 2 * NQS - stuff->cm_processed) {
    chars_to_copy = 2 * NQS - stuff->cm_processed;
  }
  memcpy(&stuff->first_quads[stuff->cm_processed],
    &stuff->read_buf[stuff->read_processed],
    chars_to_copy);
  stuff->read_processed += chars_to_copy;
  stuff->cm_processed += chars_to_copy;
  if (stuff->cm_processed < 2 * NQS)
    return 1;  // 1 means that we didn't reach 2 * NQS yet
  return 0;
}

int Server::RCInstr2Ld_(int fd, SocketTCP4 *sock, SocketStuff *stuff) {
  int chars_to_copy = stuff->read_len - stuff->read_processed;
  if (chars_to_copy > 3 * NQS - stuff->cm_processed) {
    chars_to_copy = 3 * NQS - stuff->cm_processed;
  }
  memcpy(&stuff->first_quads[stuff->cm_processed],
    &stuff->read_buf[stuff->read_processed],
    chars_to_copy);
  stuff->read_processed += chars_to_copy;
  stuff->cm_processed += chars_to_copy;
  if (stuff->cm_processed < 3 * NQS)
    return 1;  // 1 means that we didn't reach 2 * NQS yet
  return 0;
}
int Server::RCExecInstr_(int fd, SocketTCP4 *sock, SocketStuff *stuff) {
  int fn_ret = 0;
  if (stuff->supp.GetFn()) {
    fn_ret = stuff->supp.GetFn()(this, fd, sock, stuff);
  } else {
    std::cerr << "Nie ma funcksji w tym czymś, chociaż wybór się udał xd\n";
  }
  if (fn_ret < 0) {
    std::cerr << "W funkcji od obsługi instrukcji wystąpił jakiś błąd :<\n";
  } else if (fn_ret > 0) {
    // Nie doczytało :<
  }
  std::cerr << "RCExecInstr_ - skończono instrukcję.\n";
  RCResetCm_(fd, sock, stuff);
  return 0;
}

int Server::RCChooseFn_(int fd, SocketTCP4 *sock, SocketStuff *stuff) {
  InstrId instr_id(stuff->instr());
  if (instructions.count(instr_id) < 1) {
    std::cerr << "Nie ma instrukcji " << stuff->instr() << " :< \n";
    stuff->supp = InstrSupp();
    return -1;
  }
  const InstrSupp *supp = &instructions.at(instr_id);
  if (supp->Expands()) {
    int pom = RCInstr2Ld_(fd, sock, stuff);
    if (pom < 0) {
      std::cerr << "Błąd przy doczytywaniu do wybieranka.\n";
      return pom;
    }
    if (pom > 0) {
      std::cerr << "nie doczytało \n";
      return 1;
    }
    instr_id = InstrId(stuff->instr(), stuff->instr2());
    if (instructions.count(instr_id) < 1) {
      std::cerr << "Nie ma instrukcji " << stuff->instr() << ' '
        << stuff->instr2() << " :< \n";
      stuff->supp = InstrSupp();
      return -1;
    }
    supp = &instructions.at(instr_id);
  }
  if (!supp->GetFn()) {
    std::cerr << "Nie ma funkcji lel xd w instrukcji " << stuff->instr();
    if (stuff->instr2()) {
      std::cerr << ' ' << stuff->instr2() << "\n";
    } else {
      std::cerr << "\n";
    }
    stuff->supp = InstrSupp();
    return -1;
  } else {
    stuff->supp = *supp;
    stuff->strct
      = reinterpret_cast<InstrStruct *>(malloc(stuff->supp.GetSize()));
    ConstructFn fn = stuff->supp.GetConstructor();
    if (fn) {
      stuff->supp.GetConstructor()(stuff->strct);
    }
  }
  return 0;
}

int Server::DealWithReadBuf_(int fd) {
  SocketTCP4 *sock = &client_socks_.at(fd).first;
  SocketStuff *stuff = &client_socks_.at(fd).second;

  std::cerr << "int Server::DealWithReadBuf_(int fd)\n";

  int pom;
  while (stuff->read_processed < stuff->read_len) {
    std::cerr << ":\nchary w komunikacie: " << stuff->cm_processed
      << "\nprzetworzone chary z gniazda: " << stuff->read_processed
      << "\nchars read: " << stuff->read_len
      << '\n';
    if (stuff->cm_processed < 0) {
      std::cerr << "Jakiś okropny błąd :<\n";
      return - 100;
    }
    if (stuff->cm_processed < NQS) {
      pom = RCMagic_(fd, sock, stuff);
      if (pom > 0)
        return 0;
      if (pom < 0)
        return pom;
    }
    if (stuff->cm_processed < 2 * NQS) {
      pom = RCInstrLd_(fd, sock, stuff);
      if (pom > 0)
        return 0;
      if (pom < 0)
        return pom;
    }
    if (stuff->supp.Blank()) {
      std::cerr << "Nie mieliśmy insttukcji, a chcemy mieć, ok\n";
      pom = RCChooseFn_(fd, sock, stuff);
      if (pom > 0) {
        // Nie doczytało :<
        return 1;
      } else if (pom < 0) {
        return pom;
      }
    }
    pom = RCExecInstr_(fd, sock, stuff);
    if (pom > 0) {
      std::cerr << "xd\n";
      return 0;
    } else if (pom < 0) {
      return pom;
    }
    // RCResetCm_(fd, sock, stuff);
    std::cerr << "O, wygląda na to, że skończono czytać instrukcję.\n";
  }  // while
  std::cerr << "No to ten koniec czytanuia\n";
  return 0;
}


// "OwO!jabłka xd źle cos\n"



int Server::DropSock_(int fd) {
  std::cerr << "Próba dropnięcioa socketu " << fd << "\n";
  if (socks_to_users_.count(fd) > 0) {
    DeassocSock_(fd);
  }
  RCResetCm_(fd, &client_socks_.at(fd).first, &client_socks_.at(fd).second);
  client_socks_.erase(fd);
  return 0;
}


void Server::DeassocSock_(int fd) {
  std::cerr << "Odłączanie sesji od socketu " << fd << '\n';
  socks_to_users_.at(fd)->ClrSock();
  socks_to_users_.erase(fd);
}


void Server::DeassocSess_(SessionId sid) {
  TempIO q(&std::cerr);
  int fd = sess_to_users_.at(sid)->GetSock();
  std::cerr << "Odłączanie sesji " << std::hex << sid << " od gniazda "
    << std::dec << fd << ".\n";
  sess_to_users_.at(sid)->ClrSock();
  socks_to_users_.erase(fd);
}


void Server::SpecialHardcodeInit() {
  AddSession(0x3131313131313131, Username("fajny ziom"));  // '1'
  AddSession(0x3232323232323232, Username("Maciek69"));  // '2'
  AddSession(0x3333333333333333, Username("twoja stara"));  // '3'
  AddSession(0x6e6e6e6e6e6e6e6e, Username("norbs"));  // 'n'
  AddSession(0x5757575757575757, Username("Wiedzmin 997"));  // 'W'
  AddSession(0x6565656565656565, Username("eiti pw"));  // 'e'
  AddSession(0x6b6b6b6b6b6b6b6b, Username("konfident"));  // 'k'
  AddSession(0x5f5f5f5f5f5f5f5f, Username("___ __ _ xd"));  // '_'
  AddSession(0x7070707070707070, Username("paulincia"));  // 'p'
  AddSession(0x6d6d6d6d6d6d6d6d, Username("marcinPL"));  // 'm'
  AddSession(0x4242424242424242, Username("BAKSIUv13"));  // 'B'
  AddSession(0x7373737373737373, Username("smog"));  // 's'
  AddSession(0x7272727272727272, Username("repozytorium1"));  // 'r'
  AddSession(0x4c4c4c4c4c4c4c4c, Username("LubiePierogi"));  // 'L'
  AddSession(0x5454545454545454, Username("Teemo"));  // 'T'
  AddSession(0x7777777777777777, Username("Mistrz Windowsa"));  // 'w'
  AddSession(0x4747474747474747, Username("Gaben"));  // 'G'
  AddSession(0x4747474747474747, Username("pomaranczka"));  // 'G'
  AddSession(0x7878787878787878, Username("Teemo"));  // 'x'
  AddSession(0x3535353535353535, Username("mrrrauuu  hehe"));  // '5'
  AddSession(0x6161616161616161, Username("ZabujcaZla"));  // 'a'
  AddSession(0x3939393939393939, Username("djiwd9qwdk"));  // '9'
  AddSession(0x4b4b4b4b4b4b4b4b, Username("jestem kotem"));  // 'K'
  AddSession(0x2020202020202020, Username("1 2 3"));  // ' '
  AddSession(0x3030303030303030, Username("1_1"));  // '0'
  AddSession(0x4a4a4a4a4a4a4a4a, Username("Samurai Jack"));  // 'J'
}

int Server::AddSession(SessionId sid, const Username &name) {
  std::ios ios_state(nullptr);
  ios_state.copyfmt(std::cerr);
  std::cerr << "Dodawanie sesji: \"" << name << "\" : " << std::hex
    << sid << "\n";
  if (sid == 0) {
    std::cerr << "Nie można dodać takiej sesji, bo 0 nie może być :<\n";
    std::cerr.copyfmt(ios_state);
    return -3;
  }
  if (!(Username::GOOD & name.GetState())) {
    std::cerr << "Nie można dodać takiej sesji, bo nazwa jest zła: "
      << name.GetState() << '\n';
    std::cerr.copyfmt(ios_state);
    return -4;
  }
  if (users_.count(name) > 0) {
    std::cerr << "Użytkownik o takiej nazwie jest już zalogowany na sesji "
      << std::hex << users_.at(name).GetSession() << ".\n";
    std::cerr.copyfmt(ios_state);
    return -1;
  } else if (sess_to_users_.count(sid) > 0) {
    std::cerr << "Ta sesja jest już zajęta przez użytkownika o nazwie \""
      << sess_to_users_.at(sid)->GetName() << "\".\n";
    std::cerr.copyfmt(ios_state);
    return -2;
  } else {
    auto emplace_ret
      = users_.emplace(name, LoggedUser(name, sid));
    if (!emplace_ret.second) {
      std::cerr << "Yyyy, czemu???\n";
      std::terminate();
    }
    LoggedUser *ptr = &emplace_ret.first->second;
    sess_to_users_.emplace(sid, ptr);
    std::cerr << "Udało się dodać :>\n";
  }
  std::cerr.copyfmt(ios_state);
  return 0;
}

void Server::DelSession(SessionId sid) {
  TempIO q(&std::cerr);
  std::cerr << "Próba usunięcia sesji " << std::hex << sid << '\n';
  if (sess_to_users_.count(sid) < 1) {
    std::cerr << "Nie ma takiej sesji :<\n";
    return;
  }
  LoggedUser &user = *sess_to_users_.at(sid);
  int sock_fd = user.GetSock();
  const Username &name = user.GetName();
  if (sock_fd >= 0) {
    socks_to_users_.erase(sock_fd);
  }
  sess_to_users_.erase(sid);
  users_.erase(name);
}

int Server::AssocSessWithSock(SessionId sid, int fd) {
  TempIO q(&std::cerr);
  std::cerr << "Próbuję łączyć gniazdo " << std::dec << fd << " z sesją "
    << std::hex << sid << ".\n";

  // Sprawdźmy, czy sesja, do której chcemy się połączyć, istnieje.
  if (sess_to_users_.count(sid) < 1) {
    std::cerr << "Nie ma takiej sesji\n";
    return -1;
  }
  LoggedUser &user = *sess_to_users_.at(sid);
  const Username &name = user.GetName();

  // Sprawdźmy, czy ten fd nie ma już jakiejś sesji.
  if (socks_to_users_.count(fd) > 0) {
    SessionId sid2 = socks_to_users_.at(fd)->GetSession();
    Username un = socks_to_users_.at(fd)->GetName();
    std::cerr << "Socket ma już zajętą sesję: " << sid2 << " : " << un << '\n';
    return -2;
  }

  // Tutaj sprawdzimy, czy sesja, którą chcemy zająć nie jest już zajęta.
  int old_fd = user.GetSock();
  if (old_fd >= 0) {
    std::cerr << "Użytkownik \"" << name << "\" ta sesja jest już zajęta\n"
      << "przez gniazdo " << std::dec << old_fd << ", ale "
      << "na razie nic z tym nie robię :(\n";
    return -3;
  }

  // Wszystko dobrze, zajmujemy.
  auto emplace_ret = socks_to_users_.emplace(fd, &user);
  if (!emplace_ret.second) {
    std::cerr << "Coś jakoś nie dodało nie wiem czemu :/\n";
    std::terminate();
    return -4;
  }
  user.SetSock(fd);

  // ok
  std::cerr << "Gniazdo " << std::dec << fd << " zajęło sesję [ " << std::hex
    << sid << " ; '" << name << "' ]\n";
  return 0;
}

int Server::LoopTick_() {
  int feed_sel_ret = FeedSel_();
  int do_sel_ret = DoSel_();
  int read_main_fds_ret = ReadMainFds_();
  if (read_main_fds_ret > 0)
    return 0;
  // int write_to_socks_ret = WriteToSocks_();
  int read_clients_ret = ReadClients_();
  int delete_marked_socks_ret = DeleteMarkedSocks_();
  (void)feed_sel_ret;
  (void)do_sel_ret;
  (void)read_main_fds_ret;
  // (void)write_to_socks_ret;
  (void)read_clients_ret;
  (void)delete_marked_socks_ret;

  return 0;
}

int Server::DeleteMarkedSocks_() {
  auto it = client_socks_.begin();
  while (it != client_socks_.end()) {
    if (it->second.second.marked_to_delete) {
      int fd = it->second.first.GetFD();
      ++it;
      DropSock_(fd);
    } else {
      ++it;
    }
  }
  return 0;
}
}  // namespace tin
