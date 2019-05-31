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
#include "core/logger.h"

#include "send_msgs/sig.h"
#include "send_msgs/log_ok.h"
#include "send_msgs/log_off.h"

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
  LogM << "Port: " << port << '\n';
  int init_ret = InitializeListener_(port, queue_size);
  if (init_ret < 0) {
    LogH << "Nie udało się zainicjalizować listenera.\n";
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
    LogH << "Przyszło zamknięcie ze specjanego potoku.\n";
    runs_ = false;
    return 1;
  }
  if (DEAL_WITH_STDIN && (Sel::READ & sel_.Get(STDIN_FD))) {
    std::cin.getline(in_buf, IN_BUF_SIZE);
    if (std::cin.eof()) {
      LogH << "Na stdin przyszedł koniec, zamykanko :>\n";
      runs_ = false;
    } else if (std::cin.good()) {
      LogM << "Na stdin wpisano:\n" << in_buf << '\n';
      DealWithStdinBuf_(in_buf);
    } else if (std::cin.bad()) {
      LogH << "Przyszło coś na stdin, ale coś było nie tak, jak trzeba, "
        << " pomijam.\n";
    }
    std::cin.clear();
    return 1;
  }
  if (Sel::READ & sel_.Get(listening_sock_.GetFD())) {
    LogH << "Nowe połącznoko :>\n";
    SocketTCP4 sock;
    int acc_ret = listening_sock_.Accept(&sock);
    if (acc_ret < 0) {
      LogH << "Jakiś błąd przy accepcie :<\n";
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

int Server::MsgsToBufs_() {
  OutMessage *msg = nullptr;
  int added_to_buf;
  while ((msg = FirstMsg_()) != nullptr) {
    switch (msg->Audience()) {
      case OutMessage::BROADCAST_S:
        std::cerr << "OutMessage::BROADCAST_S\n";
        for (auto &x : client_socks_) {
          added_to_buf = msg->AddToBuf(&x.second.WrBuf());
          if (added_to_buf < 0) {
            std::cerr << "Błąd przy dodawaniu wiadomości do bufora. :<\n" <<
              "Socket o id " << x.first << " i fd " <<
              x.second.GetSocket().GetFD() << '\n';
            if (socks_to_users_.count(x.first) > 0) {
              std::cerr << "Jest na nim zalogowany user [[" <<
                socks_to_users_.at(x.first)->GetName() << "]]\n";
            } else {
              std::cerr << "Niezalogowane gniazdo :>\n";
            }
            x.second.ForceRemove();
            // Force, bo już i tak nic nie napiszemy raczej
          }
        }
        break;
      case OutMessage::BROADCAST_U:
        std::cerr << "OutMessage::BROADCAST_U jeszcze nie działa xd\n";
        std::terminate();
      case OutMessage::LIST_S:
        std::cerr << "OutMessage::LIST_S jeszcze nie działa xd\n";
        std::terminate();
      case OutMessage::LIST_U:
        std::cerr << "OutMessage::LIST_U jeszcze nie działa xd\n";
        std::terminate();
      case OutMessage::ONE_S:
        std::cerr << "OutMessage::ONE_S\n";
        {
          SockId id = msg->Sock();
          if (client_socks_.count(id) < 1) {
            std::cerr << "Lol, nie ma gniazda o id " << id << " xd\n";
            break;
          }
          auto &stuff = client_socks_.at(id);
          added_to_buf = msg->AddToBuf(&stuff.WrBuf());
          if (added_to_buf < 0) {
            std::cerr << "Błąd przy dodawaniu wiadomości do bufora. :<\n" <<
              "Socket o id " << id << " i fd " <<
              stuff.GetSocket().GetFD() << '\n';
            if (socks_to_users_.count(id) > 0) {
              std::cerr << "Jest na nim zalogowany user [[" <<
                socks_to_users_.at(id)->GetName() << "]]\n";
            } else {
              std::cerr << "Niezalogowane gniazdo :>\n";
            }
            stuff.ForceRemove();
            // Force, bo już i tak nic nie napiszemy raczej
          }
        }
        break;
      case OutMessage::ONE_U:
        std::cerr << "OutMessage::ONE_U jeszcze nie działa xd\n";
        std::terminate();
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



int Server::DropSock_(SockId id) {
  std::cerr << "Próba dropnięcioa socketu id " << id << "\n";
  if (client_socks_.count(id) < 1) {
    std::cerr << "Nie ma takiego socketu, wychodzonko\n";
    return -1;
  }
  if (socks_to_users_.count(id) > 0) {
    std::cerr << "Na tym gnioeździe jest ktoś zalogowany\n";
    Username un = socks_to_users_.at(id)->GetName();
    std::cerr << "[[" << un << "]]\n";
    users_.erase(un);
    socks_to_users_.erase(id);
  }
  client_socks_.erase(id);
  std::cerr << "drop: ok, wychodzonko\n";
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
  int msgs_to_bufs_ret = MsgsToBufs_();
  int delete_marked_socks_ret = DeleteMarkedSocks_();
  {
    (void)feed_sel_ret;
    (void)do_sel_ret;
    (void)msgs_to_bufs_ret;
    (void)read_main_fds_ret;
    (void)write_to_socks_ret;
    (void)read_clients_ret;
    (void)delete_marked_socks_ret;
  }

  return 0;
}

int Server::LogInUser(const Username &un, const std::string &pw,
    SockId sock_id, bool generate_response) {
  if (socks_to_users_.count(sock_id) > 0) {
    std::cerr << "gniazdo o id " << sock_id << " i fd " <<
      client_socks_.at(sock_id).GetSocket().GetFD() << " jest już zalogowane\n";
    if (generate_response)
      PushMsg_(std::unique_ptr<OutMessage>(
        new Sig(sock_id, MQ::ERR_WAS_LOGGED, false,
        "no już sie zalogowałes no")));
    return -1;
  }
  if (!un.Good()) {
    std::cerr << "Podana nazwaw jest niepoprawna!!!\n";
    if (generate_response)
      PushMsg_(std::unique_ptr<OutMessage>(
        new Sig(sock_id, MQ::ERR_BAD_LOG, false,
        "nie można mieć takiej nazwy")));
    return -1;
  }
  auto it = users_.find(un);
  if (it != users_.end()) {
    std::cerr << "user [[" << un << "]] jest zalogowany już\n"
      << "[[" << it->first << "]]\n";
    if (generate_response)
      PushMsg_(std::unique_ptr<OutMessage>(
        new Sig(sock_id, MQ::ERR_ACC_OCCUPIED, false)));
    return -1;
  }
  auto emplace_ret = users_.emplace(un, std::move(LoggedUser(un, sock_id)));
  if (emplace_ret.second == false) {
    std::cerr << "ojej, nie udało się dodać usera do mapy :\n";
    if (generate_response)
      PushMsg_(std::unique_ptr<OutMessage>(
        new Sig(sock_id, MQ::ERR_OTHER, false)));
    return -1;
  }
  LoggedUser *lu = &emplace_ret.first->second;
  auto emplace_ret2 = socks_to_users_.emplace(sock_id, lu);
  if (emplace_ret2.second == false) {
    std::cerr << "nie udało się uzupełnić mapy z id socketa na userów nowym "
      "userem :<\n";
    auto rm = users_.erase(un);
    if (generate_response)
      PushMsg_(std::unique_ptr<OutMessage>(
        new Sig(sock_id, MQ::ERR_OTHER, false)));
    assert(rm == 1);
    return -1;
  }
  std::cerr << "zalogowano [[" << un << "]]\n";
  if (generate_response)
    PushMsg_(std::unique_ptr<OutMessage>(new LogOk(sock_id)));
  return 0;
}

int Server::LogOutUser(SockId id, bool generate_response) {
  std::cerr << "Próba wylogowania socketu " << id << '\n';
  if (socks_to_users_.count(id) < 1) {
    std::cerr << "Ten socket nie jest zalogowany.\n";
    if (client_socks_.count(id) < 1) {
      std::cerr << "Nawet go nie ma xd\n";
    }
    if (generate_response)
      PushMsg_(std::unique_ptr<OutMessage>(
        new Sig(id, MQ::ERR_NOT_LOGGED, false)));
    return -1;
  }
  Username un = socks_to_users_.at(id)->GetName();
  int rm = users_.erase(un);
  assert(rm == 1);
  rm = socks_to_users_.erase(id);
  assert(rm == 1);
  std::cerr << "Na tym sockecie był zalogowany [[" << un << "]]\n";
  if (generate_response)
    PushMsg_(std::unique_ptr<OutMessage>(new LogOff(id)));
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
