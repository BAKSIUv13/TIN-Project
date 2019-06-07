// Copyright 2019 TIN

#include "core/server.h"

#include <unistd.h>

#include <set>
#include <vector>
#include <iostream>
#include <cstring>
#include <ios>
#include <utility>
#include <ctime>
#include <algorithm>
#include <bitset>

#include "core/socket_tcp4.h"
#include "core/nquad.h"
#include "core/mquads.h"
#include "core/logger.h"

#include "send_msgs/sig.h"
#include "send_msgs/log_ok.h"
#include "send_msgs/log_off.h"
#include "send_msgs/user_msg.h"
#include "send_msgs/user_status.h"

static constexpr int STDIN_FD = STDIN_FILENO;

namespace tin {

namespace {

inline intptr_t calculate_remaining_msgs(intptr_t from, intptr_t next_no,
    intptr_t n) {
  static constexpr intptr_t mod = Server::MESG_QUE_LEN;
  intptr_t i = from, res = 0;
  while (/*n >= 0 &&*/ i != next_no) {
    // n = (n + mod - 1) % mod;
    i = (i + 1) % mod;
    ++res;
  }
  if (n < res)
    return 0;
  return res;
}

}

Server::Server()
    : runs_(false), next_sock_id_(1), queued_msgs_(0), next_msg_it_(0) {
  if (pipe(end_pipe_)) {
    LogH << "Serwer się nie zrobił bo potok się nie otworzył :< "
      << std::strerror(errno) << '\n';
    throw std::runtime_error("Server couldn't create a pipe.");
  }
  am_.FeedRand(std::time(nullptr) ^ getpid() ^ end_pipe_[0]);
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
  LogL << "Numer gniazda słuchającego: " << listening_sock_.GetFD()
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
    if (sock_it->second.ShallRead(next_msg_it_, queued_msgs_)) {
      sel_.AddFD(sock_it->second.GetSocket().GetFD(), Sel::READ);
    }
    if (sock_it->second.ShallWrite(next_msg_it_, queued_msgs_)) {
      sel_.AddFD(sock_it->second.GetSocket().GetFD(), Sel::WRITE);
    }
  }
  return 0;
}

int Server::RegisterSockFromAccept_(SocketTCP4 &&sock) {
  SockId id = NextSockId_();
  auto emplace_ret = client_socks_.emplace(id,
    SocketStuff {this, id, std::move(sock), next_msg_it_});
  if (!emplace_ret.second) {
    LogH << "Nie udało się dodać socketu do mapy z socketami :<\n"
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

int Server::DoWorldWork_() {
  Username un;
  const std::string *str_ptr;
  while (world_.NextMsg(&un, &str_ptr) == 0) {
    PushMsg<UserMsg>(un, *str_ptr);
  }
  return 0;
}

Username Server::SockToUn(SockId id) {
  try {
    return socks_to_users_.at(id)->GetName();
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
  int sockets_written = 0;
  for (auto it = client_socks_.begin(); it != client_socks_.end(); ++it) {
    if (!(it->second.ShallWrite(next_msg_it_, queued_msgs_))
        || !(Sel::WRITE & sel_.Get(it->second.GetSocket().GetFD()))) {
      continue;
    }
    int res = WriteToOneSock_(&it->second);
    if (res >= 0) ++sockets_written;
  }
  return sockets_written;
}

bool Server::CheckIfSendMsg_(const SocketStuff *connection,
    const OutMessage *msg) {
  switch (msg->Audience()) {
    case OutMessage::BROADCAST_S:
      return true;
    case OutMessage::BROADCAST_U:
      return SockToUn(connection->GetId()).Good();
    case OutMessage::ONE_S:
      return msg->Sock() == connection->GetId();
    case OutMessage::ONE_U: {
      Username un = SockToUn(connection->GetId());
      return un.Good() && un == msg->User();
    }
    case OutMessage::LIST_S: case OutMessage::LIST_U: default:
      // Not implemented
      return false;
  }
}

int Server::ReadClients_() {
  int sockets_read = 0;
  for (auto it = client_socks_.begin(); it != client_socks_.end(); ++it) {
    if (!(it->second.ShallRead(next_msg_it_, queued_msgs_))
         || !(Sel::READ & sel_.Get(it->second.GetSocket().GetFD()))) {
       continue;
    }
    int read_chars = it->second.ReadCharsFromSocket();
    if (read_chars < 0) {
      LogM << "Błąd czytania sock' " << it->first <<
        " " << it->second.GetSocket().GetFD() << '\n';
      LogOutUser(it->first, false);
      it->second.Remove();
      continue;
    } else if (read_chars == 0) {
      LogM << "Zam' przyszło " << it->first <<
        " " << it->second.GetSocket().GetFD() << '\n';
      LogOutUser(it->first, false);
      it->second.Remove();
      continue;
    }
    int deal_ret = it->second.DealWithReadBuf(&world_);
    if (deal_ret < 0) {
      LogM << "Jakiś błąd przy sock' " << it->first
        << " " << it->second.GetSocket().GetFD() << '\n';
      LogOutUser(it->first, false);
      it->second.Remove();
      continue;
    }
    ++sockets_read;
  }
  return sockets_read;
}

int Server::DropSock_(SockId id) {
  LogM << "Dropnięcie sock' " << id << "\n";
  if (client_socks_.count(id) < 1) {
    LogM << "Nie ma, wychodzonko\n";
    return -1;
  }
  if (socks_to_users_.count(id) > 0) {
    Username un = socks_to_users_.at(id)->GetName();
    LogH << "Na tym sock' jest ktoś log'\n"
      << "[" << un << "], nie powinno tak być.\n"
      << "zostanie wyrzucony, ale inni klienci mogą być powiadomieni"
      << "z opóźnieniem\n";
    users_.erase(un);
    socks_to_users_.erase(id);
    PushMsg<UserStatus>(un, MQ::USER_LOGGED_OFF);
  }
  intptr_t msg = client_socks_.at(id).GetMsgPlace()[0];
  intptr_t msgs_to_lower = calculate_remaining_msgs(msg, next_msg_it_,
    queued_msgs_);
  for (intptr_t i = 0; i < msgs_to_lower; ++i) {
    intptr_t msg_index = (msg + i) % MESG_QUE_LEN;
    if (--msg_queue_[msg_index].sockets_remaining < 1) {
      assert(msg_queue_[msg_index].sockets_remaining >= 0);
      PopMsg_();
    }
  }
  client_socks_.erase(id);
  LogM << "drop: ok, wychodzonko\n";
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
  int do_world_work_ret = DoWorldWork_();
  // int msgs_to_bufs_ret = MsgsToBufs_();
  int delete_marked_socks_ret = DeleteMarkedSocks_();
  {
    (void)feed_sel_ret;
    (void)do_sel_ret;
    // (void)msgs_to_bufs_ret;
    (void)read_main_fds_ret;
    (void)write_to_socks_ret;
    (void)read_clients_ret;
    (void)do_world_work_ret;
    (void)delete_marked_socks_ret;
  }

  return 0;
}

int Server::LogInUser(Username un, const std::string &pw,
    SockId sock_id, bool generate_response) {
  // Generate response - chodzi o odpowiedź dokłądnie temu, kto sie zalogował.
  if (socks_to_users_.count(sock_id) > 0) {
    LogM << "gniazdo " << sock_id << " " <<
      client_socks_.at(sock_id).GetSocket().GetFD() << " jest już zalogowane\n";
    if (generate_response)
      PushMsg<Sig>(sock_id, MQ::ERR_WAS_LOGGED, false,
        "no już sie zalogowałes no");
    return -1;
  }
  if (!un.Good()) {
    LogM << "Podana nazwaw jest niepoprawna!!!\n";
    if (generate_response)
      PushMsg<Sig>(sock_id, MQ::ERR_BAD_LOG, false,
        "nie można mieć takiej nazwy");
    return -1;
  }
  int auth = Auth_(&un, pw);
  if (auth < 0) {
    LogH << "Błąd przy sprawdzaniu hasła\n";
    PushMsg<Sig>(sock_id, MQ::ERR_OTHER, false);
    return -1;
  } else if (auth == AccountManager::UserPass::NOT_ALLOWED) {
    LogH << "Złe dane logowania\n";
    PushMsg<Sig>(sock_id, MQ::ERR_BAD_LOG, false, "złe dane logowania");
    return -1;
  }
  auto it = users_.find(un);
  if (it != users_.end()) {
    LogM << "user [" << un << "] jest zalogowany już\n"
      << "[" << it->first << "]\n";
    if (generate_response)
      PushMsg<Sig>(sock_id, MQ::ERR_ACC_OCCUPIED, false);
    return -1;
  }
  LoggedUser::Mode mode =
    auth == AccountManager::UserPass::ADMIN ? LoggedUser::Mode::ADMIN :
    auth == AccountManager::UserPass::NORMAL ? LoggedUser::Mode::NORMAL :
    LoggedUser::Mode::GUEST;
  auto emplace_ret =
    users_.emplace(un, std::move(LoggedUser(un, sock_id, mode)));
  if (emplace_ret.second == false) {
    LogH << "ojej, nie udało się dodać usera do mapy :<\n";
    if (generate_response)
      PushMsg<Sig>(sock_id, MQ::ERR_OTHER, false);
    return -1;
  }
  LoggedUser *lu = &emplace_ret.first->second;
  auto emplace_ret2 = socks_to_users_.emplace(sock_id, lu);
  if (emplace_ret2.second == false) {
    LogH << "nie udało się uzupełnić mapy z id socketa na userów nowym "
      "userem :<\n";
    auto rm = users_.erase(un);
    if (generate_response)
      PushMsg<Sig>(sock_id, MQ::ERR_OTHER, false);
    assert(rm == 1);
    return -1;
  }
  LogM << "zalogowano [" << un << "]\n";
  world_.AddArtist(un);
  if (generate_response)
    PushMsg<LogOk>(sock_id);
  PushMsg<UserStatus>(un, MQ::USER_LOGGED_IN);
  return 0;
}

int Server::LogOutUser(SockId id, bool generate_response) {
  LogM << "Wylogowywanie sock' " << id << '\n';
  if (socks_to_users_.count(id) < 1) {
    LogM << "Sock' nie jest log'\n";
    if (client_socks_.count(id) < 1) {
      LogM << "Nawet go nie ma xd\n";
    }
    if (generate_response)
      PushMsg<Sig>(id, MQ::ERR_NOT_LOGGED, false);
    return -1;
  }
  Username un = socks_to_users_.at(id)->GetName();
  int rm = users_.erase(un);
  assert(rm == 1);
  rm = socks_to_users_.erase(id);
  assert(rm == 1);
  LogM << "Na tym sockecie był zalogowany [" << un << "]\n";
  if (generate_response)
    PushMsg<LogOff>(id);
  PushMsg<UserStatus>(un, MQ::USER_LOGGED_OFF);
  return 0;
}

int Server::DeleteMarkedSocks_() {
  auto it = client_socks_.begin();
  while (it != client_socks_.end()) {
    if (it->second.ShallBeRemoved(next_msg_it_, queued_msgs_)) {
      SockId id = it->first;
      ++it;
      DropSock_(id);
    } else {
      ++it;
    }
  }
  return 0;
}



int Server::PopMsg_() {
  if (queued_msgs_ < 1) {
    return - 1;
  }
  size_t msg_index =
    (MESG_QUE_LEN + next_msg_it_ - queued_msgs_) % MESG_QUE_LEN;
  msg_queue_[msg_index].msg.reset();
  msg_queue_[msg_index].str.clear();
  msg_queue_[msg_index].sockets_remaining = 0;
  --queued_msgs_;
  return 0;
}

int Server::DisposeMsg_() {
  assert(queued_msgs_ == MESG_QUE_LEN - 1);
  assert((MESG_QUE_LEN + next_msg_it_ - queued_msgs_) % MESG_QUE_LEN ==
    (next_msg_it_ + 1) % MESG_QUE_LEN);
  int cut_connections = 0;
  intptr_t msg_index = (next_msg_it_ + 1) % MESG_QUE_LEN;
  for (auto &connection : client_socks_) {
    SockId id = connection.first;
    SocketStuff &stuff = connection.second;
    if (stuff.GetMsgPlace()[0] == msg_index) {
      // Połączenie jest za wolne, zrywamy gniazdo.
      Username un = SockToUn(id);
      if (un) {
        LogOutUser(id, false);
      }
      stuff.ForceRemove();
      ++cut_connections;
    }
  }
  PopMsg_();
  return cut_connections;
}

int Server::UserDel(const Username &un) {
  if (!am_.Writable()) return -1;
  LoggedUser::Mode old_mode = LoggedUser::Mode::NOTHING;
  if (users_.count(un) > 0) {
    old_mode = users_.at(un).GetMode();
    users_.at(un).ChMode(LoggedUser::Mode::GUEST);
  }
  int res = am_.UserDel(un);
  if (res < 0 && old_mode != LoggedUser::Mode::NOTHING) {
    users_.at(un).ChMode(old_mode);
    return -1;
  }
  return 0;
}


int Server::UserChRole(const Username &un, bool admin) {
  if (!am_.Writable()) return -1;
  LoggedUser::Mode old_mode = LoggedUser::Mode::NOTHING;
  if (users_.count(un) > 0) {
    old_mode = users_.at(un).GetMode();
    users_.at(un).ChMode
      (admin ? LoggedUser::Mode::ADMIN : LoggedUser::Mode::NORMAL);
  }
  int res = am_.UserChRole(un, admin);
  if (res < 0 && old_mode != LoggedUser::Mode::NOTHING) {
    users_.at(un).ChMode(old_mode);
    return -1;
  }
  return 0;
}


int Server::UserChPasswd(const Username &un, const std::string &passwd) {
  if (!am_.Writable()) return -1;
  FeedRand_();
  int res = am_.UserChPasswd(un, passwd);
  return res;
}


int Server::UserAdd(const Username &un, const std::string &passwd, bool admin) {
  if (!am_.Writable()) return -1;
  LoggedUser::Mode old_mode = LoggedUser::Mode::NOTHING;
  if (users_.count(un) > 0) {
    old_mode = users_.at(un).GetMode();
    users_.at(un).ChMode
      (admin ? LoggedUser::Mode::ADMIN : LoggedUser::Mode::NORMAL);
  }
  FeedRand_();
  int res = am_.UserAdd(un, passwd, admin);
  if (res < 0 && old_mode != LoggedUser::Mode::NOTHING) {
    users_.at(un).ChMode(old_mode);
    return -1;
  }
  return 0;
}


int Server::WriteToOneSock_(SocketStuff *stuff) {
  char buf[WRT_BUF];
  std::array<intptr_t, 2> place = stuff->GetMsgPlace();
  std::bitset<MESG_QUE_LEN> shall_send;
  intptr_t copied = 0, written = 0;
  intptr_t msg_i = 0;
  intptr_t chars_to_copy;
  intptr_t msg_offset = place[1];
  while (copied < WRT_BUF &&
      (msg_i + place[0]) % MESG_QUE_LEN != next_msg_it_) {
    intptr_t msg_number = (place[0] + msg_i) % MESG_QUE_LEN;
    MsgCell &cell = msg_queue_[msg_number];
    // Niżej sprawdzamy, czy mamy wysłać tę wiadomość na to gniazdo,
    // sprawdzenie msg_offset == 0 jest po to, że jak nie jest to nie ma po co
    // sprawdzać, bo już to wczześniej zrobiliśmy.
    if (msg_offset == 0 && !CheckIfSendMsg_(stuff, &*cell.msg)) {
      chars_to_copy = 0;
      shall_send[msg_i] = false;
    } else {
      chars_to_copy =
        std::min(WRT_BUF - copied,
          (intptr_t)msg_queue_[msg_number].str.size() - msg_offset);
      memcpy(&buf[copied], &msg_queue_[msg_number].str.c_str()[msg_offset],
        chars_to_copy);
      copied += chars_to_copy;
      shall_send[msg_i] = true;
    }
    ++msg_i;
    msg_offset = 0;
  }
  if (copied > 0) {
    written = write(stuff->GetSocket().GetFD(), buf, copied);
    if (written < 0) {
      // Oj, bardzo źle.
      LogH << "Błąd pisania do gniazda (funkcja write), " << stuff->GetId() << ' '
        << stuff->GetSocket().GetFD() << '\n' << std::strerror(errno) << '\n';
      stuff->ForceRemove();
      return -2;
    }
    if (written == 0) {
      // chyba zamknięte
      LogVL << "gniazdo zamknięte??\n";
      stuff->Remove();
      return -1;
    }
  }
  intptr_t msg_bytes;
  msg_offset = place[1];
  intptr_t seen_msgs = msg_i;
  intptr_t to_subtract = 0;
  msg_i = 0;
  while (msg_i < seen_msgs) {
    bool ignore = false;
    intptr_t msg_number = (place[0] + msg_i) % MESG_QUE_LEN;
    if (!shall_send[msg_i]) {
      to_subtract = 0;
      ignore = true;
      msg_bytes = 0;
    } else {
      msg_bytes = msg_queue_[msg_number].str.size() - msg_offset;
      to_subtract = std::min(msg_bytes, written);
      ignore = false;
    }
    msg_offset = 0;
    written -= to_subtract;
    if (to_subtract == msg_bytes || ignore) {
      msg_queue_[msg_number].sockets_remaining -= 1;
      assert(msg_queue_[msg_number].sockets_remaining >= 0);
      if (msg_queue_[msg_number].sockets_remaining < 1) {
        PopMsg_();
      }
      ++msg_i;
    } else {
      assert(written == 0);
      break;
    }
  }
  assert(written == 0);
  intptr_t final_msg = (place[0] + msg_i) % MESG_QUE_LEN;
  intptr_t final_offset = 0;
  if (final_msg != next_msg_it_) {
    final_offset = to_subtract;
    if (place[0] == final_msg) {
      // Może nie za ładnie ale zadziała.
      final_offset += place[1];
    }
  }
  stuff->SetMsgPlace(final_msg, final_offset);
  return 1;
}


void Server::FeedRand_() {
  return am_.FeedRand(std::time(nullptr) ^ getpid() ^ client_socks_.size() ^
  reinterpret_cast<intptr_t>(this));
}

}  // namespace tin
