// Copyright 2019 Piotrek

#ifndef SERVER_APP_SOCKET_STUFF_H_
#define SERVER_APP_SOCKET_STUFF_H_

#include <unistd.h>

#include <string>
#include <queue>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>

#include "core/socket_tcp4.h"
#include "core/nquad.h"
#include "core/instr_struct.h"
#include "app/instr_supp.h"
#include "core/write_buf.h"
#include "core/mquads.h"
#include "core/instr_id.h"
#include "core/destroy_fn.h"
#include "core/sock_id.h"

// Definitions for this file for 'readability':
// - command - that string which starts with 'OwO!'
// - message - thing read by read fn

namespace tin {

class Server;
class SocketStuff {
 public:
  // Size of buffer used to read chars from socket.
  static constexpr size_t BUF_SIZE = 2048;

  static constexpr int NQS = sizeof(NQuad);

  explicit SocketStuff(Server *s, SockId id, SocketTCP4 &&socket)
      : serv_(s),
        socket_(std::move(socket)),
        shall_read_var_(false),
        shall_be_removed_(RemoveEnum::NO),
        cm_processed_(0), msg_len_(0), msg_processed_(0), strct_(nullptr),
        supp_(),
        id_(id) {
    read_buf_[BUF_SIZE] = '\0';
  }

  // Variable which tells if socket shall be closed at next 'close step'.
  enum class RemoveEnum {
    NO = 0,
    YES,
    FORCE
  };

  // Mark socket to remove.
  void Remove() {
    shall_be_removed_ = std::max(shall_be_removed_, RemoveEnum::YES);
  }

  // This fn sets socket for reading.
  void SetReading(bool x) {
    shall_read_var_ = x;
  }

  // This fn marks socket to remove even if it has sth to write.
  void ForceRemove() {
    shall_be_removed_ = RemoveEnum::FORCE;
  }

  // Tells if in next deletion step server shall close this socket.
  constexpr bool ShallBeRemoved() const {
    return shall_be_removed_ >= RemoveEnum::YES && !ShallWrite();
  }

  // This tells if socket is handled in 'read step'.
  constexpr bool ShallRead() const {
    return shall_read_var_ && !ShallWrite() && !ShallBeRemoved();
  }

  // This tells if program shall write to this socket in next 'write step'.
  constexpr bool ShallWrite() const {
    return write_buf_.Chars() > 0 && shall_be_removed_ < RemoveEnum::FORCE;
  }

/*
  constexpr int CountCopy(int how_much) const {
    return std::min(msg_len_ - msg_processed_, how_much - cm_processed_);
  }

  void Copy(void *dest, size_t how_much) {
    memcpy(dest, &read_buf_[msg_processed_], how_much);
    msg_processed_ += how_much;
    cm_processed_ += how_much;
  }
*/

  // write buf
  WriteBuf &WrBuf() {
    return write_buf_;
  }


  // This reads from socket to our buffer as much chars as it can.
  int ReadCharsFromSocket() {
    int fd = socket_.GetFD();
    ssize_t read_ret = read(fd, read_buf_, BUF_SIZE);
    if (read_ret < 0) {
      std::cerr << "Błąd przy czytaniu socketu " << fd
        << "\n errno: " << std::strerror(errno) << "\n";
      return -1;
    } else if (read_ret == 0) {
      // std::cerr << "Na gniazdo przyszło zamknięcie.\n";
      // Remove();
      // ZAMKNIĘCIE
      return 0;
    } else /* read_ret > 0 */ {
      msg_len_ = read_ret;
      msg_processed_ = 0;
      return read_ret;
    }
  }

  // Get int32 from socket.
  int ReadQuad(int at, NQuad *dest) {
    return ReadString(at, NQS, dest);
  }

  int ReadString(int start, int len, void *dest) {
    if (cm_processed_ < start) {
      std::cerr << "za daleko jest to, co chcemy skopiować\n";
      return -1;
    }
    int cp_dest_start = cm_processed_ - start;\
    int end = start + len;
    if (cm_processed_ >= end) {
      std::cerr << "za daleko już jesteśmy\n";
      return -1;
    }
    char *cp_src_ptr = &(reinterpret_cast<char *>(read_buf_)[msg_processed_]);
    char *cp_dest_ptr = dest != nullptr ?
      &(reinterpret_cast<char *>(dest)[cp_dest_start]) :
      nullptr;
    int n = CountCopy_(end);
    if (dest != nullptr) {
      memcpy(cp_dest_ptr, cp_src_ptr, n);
    }
    Shift_(n);
    if (cm_processed_ >= end) {
      return 0;
    } else {
      return 1;
    }
    return 0;
  }

  int ReadCpp11String(int start, int len, std::string *dest) {
    if (cm_processed_ < start) {
      std::cerr << "za daleko jest to, co chcemy skopiować\n";
      return -1;
    }
    int end = start + len;
    if (cm_processed_ >= end) {
      std::cerr << "za daleko już jesteśmy\n";
      return -1;
    }
    char *cp_src_ptr = &(reinterpret_cast<char *>(read_buf_)[msg_processed_]);
    int n = CountCopy_(end);
    if (dest != nullptr) {
      dest->append(cp_src_ptr, n);
    }
    Shift_(n);
    if (cm_processed_ >= end) {
      return 0;
    } else {
      return 1;
    }
    return 0;
  }

  constexpr int CmProcessed() const {
    return cm_processed_;
  }

  constexpr bool HasInstr() const {
    return !supp_.Blank();
  }

  SocketTCP4 &GetSocket() {
    return socket_;
  }

  // ile jeszczse
  constexpr int CharsLeft() const {
    return msg_len_ - msg_processed_;
  }

  int ReadMagic() {
    int x = ReadQuad(0, &magic_);
    if (x != 0) {
      return x;
    }
    if (magic_ != MQ::OWO) {
      std::cerr << "Miało być 'OwO!', a nie jest\n";
      return -1;
    }
    return 0;
  }

  int ReadInstr() {
    return ReadQuad(NQS, &instr_);
  }

  int ReadInstr2() {
    return ReadQuad(2 * NQS, &instr2_);
  }

  int ChooseInstr();

  InstrFn GetInstrFn() {
    if (supp_.Blank()) {
      return nullptr;
    }
    return supp_.GetFn();
  }

  InstrStruct *GetStrct() {
    return strct_;
  }

  SockId GetId() {
    return id_;
  }

  int ResetCommand() {
    cm_processed_ = 0;
    if (supp_ && supp_.GetFn()) {
      DestroyFn fn = supp_.GetDestructor();
      if (fn) {
        fn(strct_);
      }
      free(strct_);
      strct_ = nullptr;
      supp_ = InstrSupp();
    }
    return 0;
  }


  int DealWithReadBuf(World *w, MsgPushFn push_fn) {
    std::cerr << "int DealWithReadBuf()\n";
    std::cerr << "Gniazdo o id " << id_ << " i fd " <<
      GetSocket().GetFD() << '\n';

    int pom;
    while (CharsLeft() > 0) {
      std::cerr << "chary w komunikacie: " << CmProcessed()
        << "\nprzetworzone chary z gniazda: " << msg_processed_
        << "\nchars read: " << msg_len_
        << '\n';
      if (CmProcessed() < 0) {
        std::cerr << "Jakiś okropny błąd :<\n";
        return - 100;
      }
      if (CmProcessed() < NQS) {
        pom = ReadMagic();
        if (pom > 0)
          return 0;
        if (pom < 0)
          return pom;
      }
      if (CmProcessed() < 2 * NQS) {
        pom = ReadInstr();
        if (pom > 0)
          return 0;
        if (pom < 0)
          return pom;
      }
      if (!HasInstr()) {
        std::cerr << "Nie mieliśmy insttukcji, a chcemy mieć, ok\n";
        pom = ChooseInstr();
        if (pom > 0) {
          // Nie doczytało :<
          return 1;
        } else if (pom < 0) {
          return pom;
        }
      }
      InstrFn fn = GetInstrFn();
      pom = fn(serv_, this, w, push_fn);
    if (pom > 0) {
      std::cerr << "ExecInstr nieee fn zwróciło >0 xd\n";
      return 0;
    } else if (pom < 0) {
      return pom;
    }
    std::cerr << "O, wygląda na to, że skończono czytać instrukcję.\n";
    ResetCommand();
  }  // while
  std::cerr << "No to ten koniec czytanuia\n";
  return 0;
}

  // void CopyToCpp11String(std::string *dest, std::string::size_type how_much)
  // {
  //   dest->append(&read_buf[msg_processed], how_much);
  //   read_processed += how_much;
  //   cm_processed += how_much;
  // }

 private:

  void Shift_(int how_much) {
    cm_processed_ += how_much;
    msg_processed_ += how_much;
  }


  // Counts how many chars can we copy at this time.
  constexpr int CountCopy_(int to) const {
    return std::min(msg_len_ - msg_processed_, to - cm_processed_);
  }



  // We need this pointer to server :<
  Server *serv_;

  // Socket :3
  SocketTCP4 socket_;

  // This variable tells if socket can be read.
  bool shall_read_var_;

  // This variable tells sth about removing socket.
  RemoveEnum shall_be_removed_;

  // Number of processed chars in actually handled 'command'.
  int cm_processed_;

  // Number of chars that have been read with 'read' function last time.
  int msg_len_;

  // Number of processed chars in message from last read.
  int msg_processed_;

  // Memory to store instruction helping information.
  InstrStruct *strct_;
  // std::unique_ptr<InstrStruct, DestroyFn> strct_;

  // Info about actual instruction.
  InstrSupp supp_;

  // Buffer which takes data from read directly from socket.
  char read_buf_[BUF_SIZE + 1];

  // This has to be "OwO!".
  NQuad magic_;

  // Quad containing instruction number.
  NQuad instr_;

  // Additional quad used by some instructions.
  NQuad instr2_;

  // Buffer to send with write fn.
  WriteBuf write_buf_;

  // Id of socekt
  SockId id_;
};

}  // namespace tin

#endif  // SERVER_APP_SOCKET_STUFF_H_
