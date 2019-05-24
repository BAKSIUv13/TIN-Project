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

#include "send_msgs/sig.h"

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
}  // namespace

namespace tin {
Server::Server()
    : runs_(false), next_sock_id_(1) {
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
    std::cerr << "Nie udało się zainicjalizować listenera.\n";
    return;
  }
  bool not_exit = true;
  runs_ = true;
  while (not_exit && runs_) {
    LoopTick_();
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
    if (sock_it->second.ShallRead()) {
      sel_.AddFD(sock_it->first, Sel::READ);
    }
    if (sock_it->second.ShallWrite()) {
      sel_.AddFD(sock_it->first, Sel::WRITE);
    }
  }
  return 0;
}

int Server::RegisterSockFromAccept_(SocketTCP4 &&sock) {
  SockId id = NextSockId_();
  auto emplace_ret = client_socks_.emplace(id,
    SocketStuff {this, id});
  if (!emplace_ret.second) {
    std::cerr << "Nie udało się dodać socketu do mapy z socketami :<\n"
      "id: " << id
      << " trzeba coś z tym zrobić.\n";
    return -1;
  }
  emplace_ret.first->second.SetReading(true);
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
      DealWithStdinBuf_(in_buf);
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

Username Server::SockToUn(int fd) {
  try {
    return socks_to_users_.at(fd)->GetName();
  } catch(std::out_of_range &e) {
    return Username();
  }
}

int Server::DealWithStdinBuf_(const char *s) {
  if (strncmp(s, "stop", 4) == 0 && (s[4] == '\n' || s[4] == ' ')) {
    StopRun();
  }
  /*else if (strncmp(s, "l ", 2) == 0) {
    Username name(&s[2]);
    std::cerr << "Nazwa: [[" << name << "]] - [["
      << Username(name, Username::CONDENSE) << "]]\n";
    SessionId sid;
    try {
      sid = users_.at(name).GetSession();
    }
    catch(std::out_of_range &e) {
      std::cerr << "Nie ma sesji tego użytwkonika.\n";
      return 0;
    }
    DelSession(sid);
  }*/
  return 0;
}

int Server::StopRun() {
  return write(end_pipe_[1], "", 1);
}

int Server::WriteToSocks_() {
  static constexpr int WRT_BUF = WriteBuf::SIZE;
  char buf[WRT_BUF];
  int sockets_written = 0;
  for (auto it = client_socks_.begin(); it != client_socks_.end(); ++it) {
    if (!(it->second.ShallWrite())
        || !(Sel::WRITE & sel_.Get(it->first))) {
      continue;
    }
    WriteBuf &swb = it->second.WrBuf();
    int pom = swb.Get(buf, WRT_BUF);
    if (pom < 0) {
      // źle hehe
      it->second.Remove();
      continue;
    }
    int pom2 = write(it->first, buf, pom);
    if (pom2 < 0) std::terminate();
    if (pom2 == 0) {
      // chyba zamknięte
      std::cerr << "zamknięte??\n";
      it->second.Remove();
    }
    swb.Pop(pom2);
    ++sockets_written;
  }
  return sockets_written;
}

int Server::DoWork_() {
  OutMessage *msg = nullptr;
  int pom;
  while ((msg = FirstMsg_())) {
    if (msg->Broadcast()) {
      for (auto &x : client_socks_) {
        // Jeszcze pomińmy niezalogowane gniazda:
        if (socks_to_users_.count(x.first) < 1) {
          continue;
        }
        pom = msg->AddToBuf(&x.second.WrBuf());
        if (pom < 0) {
          std::cerr << "Błąd przy dodawaniu wiadom,ości do bufora. :<\n";
          x.second.Remove();
        }
      }
    } else {
      for (const Username& un : msg->Users()) {
        if (users_.count(un) < 1) {
          std::cerr << "Lel, nie ma takiego kogoś teraz xd\n";
          continue;
        }
        auto &stuff = client_socks_.at(users_.at(un).GetSockId());
        pom = msg->AddToBuf(&stuff.WrBuf());
        if (pom < 0) {
          std::cerr << "Błedzik [[" << un << "]]. :<\n";
          stuff.Remove();
        }
      }
    }
    PopMsg_();
  }
  return 0;
}


int Server::ReadClients_() {
  int sockets_read = 0;
  for (auto it = client_socks_.begin(); it != client_socks_.end(); ++it) {
    if (!(it->second.ShallRead())
        || !(Sel::READ & sel_.Get(it->first))) {
      continue;
    }
    // if (ReadClientSocket_(it->first) >= 0) {
    if (client_socks_.at(it->first).ReadCharsFromSocket() >= 0) {
      ++sockets_read;
    }
  }
  return sockets_read;
}

int Server::DealWithReadBuf_(int fd) {
  SocketStuff *stuff = &client_socks_.at(fd);

  std::cerr << "int Server::DealWithReadBuf_(int fd)\n";

  int pom;
  while (stuff->CharsLeft() > 0) {
    std::cerr << ":\nchary w komunikacie: " << stuff->CmProcessed()
      // << "\nprzetworzone chary z gniazda: " << stuff->read_processed_
      // << "\nchars read: " << stuff->read_len_
      << '\n';
    if (stuff->CmProcessed() < 0) {
      std::cerr << "Jakiś okropny błąd :<\n";
      return - 100;
    }
    if (stuff->CmProcessed() < NQS) {
      pom = stuff->ReadMagic();
      if (pom > 0)
        return 0;
      if (pom < 0)
        return pom;
    }
    if (stuff->CmProcessed() < 2 * NQS) {
      pom = stuff->ReadInstr();
      if (pom > 0)
        return 0;
      if (pom < 0)
        return pom;
    }
    if (stuff->HasInstr()) {
      std::cerr << "Nie mieliśmy insttukcji, a chcemy mieć, ok\n";
      pom = stuff->ChooseInstr();
      if (pom > 0) {
        // Nie doczytało :<
        return 1;
      } else if (pom < 0) {
        return pom;
      }
    }
    InstrFn fn = stuff->GetInstrFn();
    pom = fn(this, stuff, &world_, &Server::PushMsg_);
    if (pom > 0) {
      std::cerr << "ExecInstr nieee fn zwróciło >0 xd\n";
      return 0;
    } else if (pom < 0) {
      return pom;
    }
    std::cerr << "O, wygląda na to, że skończono czytać instrukcję.\n";
    stuff->ResetCommand();
  }  // while
  std::cerr << "No to ten koniec czytanuia\n";
  return 0;
}


// "OwO!jabłka xd źle cos\n"



int Server::DropSock_(int fd) {
  std::cerr << "Próba dropnięcioa socketu " << fd << "\n";
  // if (socks_to_users_.count(fd) > 0) {
  // }
  client_socks_.erase(fd);
  return 0;
}





void Server::SpecialHardcodeInit() {

}

int Server::LoopTick_() {
  int feed_sel_ret = FeedSel_();
  int do_sel_ret = DoSel_();
  int read_main_fds_ret = ReadMainFds_();
  if (read_main_fds_ret > 0)
    return 0;
  int write_to_socks_ret = WriteToSocks_();
  int read_clients_ret = ReadClients_();
  int do_work_ret = DoWork_();
  int delete_marked_socks_ret = DeleteMarkedSocks_();
  {
    (void)feed_sel_ret;
    (void)do_sel_ret;
    (void)do_work_ret;
    (void)read_main_fds_ret;
    (void)write_to_socks_ret;
    (void)read_clients_ret;
    (void)delete_marked_socks_ret;
  }

  return 0;
}

int Server::LogInUser(const Username &un, const std::string &pw,
    SockId sock_id, bool generate_response) {
  if (!un.Good()) {
    std::cerr << "Podana nazwaw jest niepoprawna!!!\n";
    PushMsg_(new Sig(un, MQ::ERR_BAG_LOG, false));
    return -1;
  }
  if (IsLogged_(un)) {
    std::cerr << "user [[" << un << "]] jest zalogowany już\n";
    return -1;
  }
  auto emplace_ret = users_.emplace(un, std::move(LoggedUser(un, sock_id)));
  LoggedUser *lu = &emplace_ret.first->second;
  socks_to_users_.emplace(sock_id, lu);
  return 0;
}

int Server::DeleteMarkedSocks_() {
  auto it = client_socks_.begin();
  while (it != client_socks_.end()) {
    if (it->second.ShallBeRemoved()) {
      int fd = it->first;
      ++it;
      DropSock_(fd);
    } else {
      ++it;
    }
  }
  return 0;
}


int Server::PushMsg_(std::unique_ptr<OutMessage> msg) {
  messages_to_send_.emplace_back(std::move(msg));
  return 0;
}


OutMessage *Server::FirstMsg_() {
  if (messages_to_send_.size() < 1) return nullptr;
  return &*messages_to_send_.front();
}

int Server::PopMsg_() {
  messages_to_send_.erase(messages_to_send_.begin());
  return 0;
}


}  // namespace tin
