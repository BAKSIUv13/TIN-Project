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
#include "send_msgs/log_ok.h"

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
      sel_.AddFD(sock_it->second.GetSocket().GetFD(), Sel::READ);
    }
    if (sock_it->second.ShallWrite()) {
      sel_.AddFD(sock_it->second.GetSocket().GetFD(), Sel::WRITE);
    }
  }
  return 0;
}

int Server::RegisterSockFromAccept_(SocketTCP4 &&sock) {
  SockId id = NextSockId_();
  auto emplace_ret = client_socks_.emplace(id,
    SocketStuff {this, id, std::move(sock)});
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
        || !(Sel::WRITE & sel_.Get(it->second.GetSocket().GetFD()))) {
      continue;
    }
    WriteBuf &swb = it->second.WrBuf();
    int loaded_from_buf = swb.Get(buf, WRT_BUF);
    if (loaded_from_buf < 0) {
      // źle hehe
      it->second.Remove();
      continue;
    }
    int written = write(it->second.GetSocket().GetFD(), buf, loaded_from_buf);
    if (written < 0) {
      // źle hehe
      it->second.Remove();
      continue;
    }
    if (written == 0) {
      // chyba zamknięte
      std::cerr << "zamknięte??\n";
      it->second.Remove();
    }
    swb.Pop(written);
    ++sockets_written;
  }
  return sockets_written;
}

int Server::DoWork_() {
  OutMessage *msg = nullptr;
  int added_to_buf;
  while ((msg = FirstMsg_()) != nullptr) {
    if (msg->Broadcast()) {
      for (auto &x : client_socks_) {
        // Jeszcze pomińmy niezalogowane gniazda:
        if (socks_to_users_.count(x.first) < 1) {
          continue;
        }
        added_to_buf = msg->AddToBuf(&x.second.WrBuf());
        if (added_to_buf < 0) {
          std::cerr << "Błąd przy dodawaniu wiadom,ości do bufora. :<\n";
          x.second.Remove();
        }
      }
    } else {
      for (const Username &un : msg->Users()) {
        if (users_.count(un) < 1) {
          std::cerr << "Lel, nie ma takiego kogoś teraz xd [[" << un << "]]\n";
          continue;
        }
        // Jest user xd.
        auto &stuff = client_socks_.at(users_.at(un).GetSockId());
        added_to_buf = msg->AddToBuf(&stuff.WrBuf());
        if (added_to_buf < 0) {
          std::cerr << "Błedzik [[" << un << "]], chyba się do bufora nie "
            "zmieściło. :<\n";
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
    // bool jeden = !(it->second.ShallRead());
    // bool dwa = !(Sel::READ & sel_.Get(it->second.GetSocket().GetFD()));
    // bool shall_not_read = jeden || dwa;
      // !(it->second.ShallRead()) || !(Sel::READ & sel_.Get(it->first));
    if (!(it->second.ShallRead())
         || !(Sel::READ & sel_.Get(it->second.GetSocket().GetFD()))) {
       continue;
    }
    // if (shall_not_read) {
    //   continue;
    // }
    // if (ReadClientSocket_(it->first) >= 0) {
    int read_chars = it->second.ReadCharsFromSocket();
    if (read_chars < 0) {
      std::cerr << "Był błąd przy czytaniu socketu o id " << it->first <<
        " i fd " << it->second.GetSocket().GetFD() << '\n';
      it->second.Remove();
      continue;
    } else if (read_chars == 0) {
      std::cerr << "Zamykanko przyszło " << it->first <<
        " i fd " << it->second.GetSocket().GetFD() << '\n';
      it->second.Remove();
      continue;
    }
    int deal_ret = it->second.DealWithReadBuf(&world_, &Server::PushMsg_);
    if (deal_ret < 0) {
      std::cerr << "Jakiś błąd przy ogarnianiu rzeczy z socketu " << it->first
        << " i fd " << it->second.GetSocket().GetFD() << '\n';
      it->second.Remove();
      continue;
    }
    ++sockets_read;
  }
  return sockets_read;
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
  return;
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
    PushMsg_(std::unique_ptr<OutMessage>(new Sig(un, MQ::ERR_BAG_LOG, false)));
    return -1;
  }
  auto it = users_.find(un);
  if (it != users_.end()) {
  // if (IsLogged_(un)) {
    std::cerr << "user [[" << un << "]] jest zalogowany już\n"
      << "[[" << it->first << "]]\n";
    PushMsg_(std::unique_ptr<OutMessage>(
      new Sig(un, MQ::ERR_WAS_LOGGED, false)));
    return -1;
  }
  auto emplace_ret = users_.emplace(un, std::move(LoggedUser(un, sock_id)));
  if (emplace_ret.second == false) {
    std::cerr << "ojej, nie udało się dodać usera do mapy :\n";
    PushMsg_(std::unique_ptr<OutMessage>(new Sig(un, MQ::ERR_OTHER, false)));
    return -1;
  }
  LoggedUser *lu = &emplace_ret.first->second;
  auto emplace_ret2 = socks_to_users_.emplace(sock_id, lu);
  if (emplace_ret2.second == false) {
    std::cerr << "nie udało się uzupełnić mapy z id socketa na userów nowym "
      "userem :<\n";
    auto rm = users_.erase(un);
    PushMsg_(std::unique_ptr<OutMessage>(new Sig(un, MQ::ERR_OTHER, false)));
    assert(rm == 1);
    return -1;
  }
  std::cerr << "zalogowano [[" << un << "]]\n";
  PushMsg_(std::unique_ptr<OutMessage>(new LogOk(un)));
  return 0;
}

int Server::DeleteMarkedSocks_() {
  auto it = client_socks_.begin();
  while (it != client_socks_.end()) {
    if (it->second.ShallBeRemoved()) {
      SockId id = it->first;
      ++it;
      DropSock_(id);
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
