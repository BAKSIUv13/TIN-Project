// Copyright 2019 Piotrek

#ifndef SERVER_APP_SOCKET_STUFF_H_
#define SERVER_APP_SOCKET_STUFF_H_

#include <string>
#include <queue>
#include <algorithm>

#include "network/nws.h"
#include "core/nquad.h"
#include "core/instr_struct.h"
#include "app/instr_supp.h"
#include "core/write_buf.h"

namespace tin {

struct SocketStuff {
  // Size of buffer used to read chars from socket.
  static constexpr size_t BUF_SIZE = 2048;
/*
  // Maximum number of segments. Now probably useless.
  static constexpr int MAX_SEGMENTS = 1024 * 1024;
*/
  // Yyyyyyy czy kończymy jak mamy źle co≥ś
  static constexpr bool END_ON_BAD_DATA = true;  // to prawdopodobnie tymczasowe

  // Error numbers pushed to queue.
  enum Error {
    OTHER = 1,
    NOT_OWO,
    AUTH_FAILED,
    SESS_CAPTURE_FAILED,
  };

  SocketStuff()
      : marked_to_delete(false), shall_read(false), shall_write(false),
        cm_processed(0), read_len(0), read_processed(0), strct(nullptr),
        supp() {
    read_buf[BUF_SIZE] = '\0';
  }

  // Buffer which takes data from read directly from socket.
  char read_buf[BUF_SIZE + 1];

  // Buffer which contains copied bytes from read data that is beginning.
  alignas(NQuad) char first_quads[3 * sizeof(NQuad)];

  // Buffer to send.
  WriteBuf write_buf;

  // Variable which tells if socket shall be closed at next 'close step'.
  bool marked_to_delete;

  // This tells if socket is handled in 'read step'.
  bool shall_read;

  // This tells if program shall write to this socket in next 'write step'.
  bool shall_write;

  constexpr bool ShallWrite() {
    return write_buf.Chars() > 0;
  }

  // Reference to first quad in 'first_quads' buffer that has to be "OwO!".
  NQuad &magic() {
    return reinterpret_cast<NQuad *>(first_quads)[0];
  }

  // Reference to quad containing instruction number.
  NQuad &instr() {
    return reinterpret_cast<NQuad *>(first_quads)[1];
  }

  // Reference to additional quad used by some instructions.
  NQuad &instr2() {
    return reinterpret_cast<NQuad *>(first_quads)[2];
  }

  // Number of processed chars in actually handled 'command'.
  int cm_processed;

  // Number of chars that have been read with 'read' function last time.
  int read_len;

  // Number of processed chars from last read.
  int read_processed;

  // Memory to store instruction helping information.
  InstrStruct *strct;

  // Info about actual instruction.
  InstrSupp supp;

  constexpr int CountCopy(int how_much) const {
    return std::min(read_len - read_processed, how_much - cm_processed);
  }

  void Copy(void *dest, size_t how_much) {
    memcpy(dest, &read_buf[read_processed], how_much);
    read_processed += how_much;
    cm_processed += how_much;
  }

  void CopyToCpp11String(std::string *dest, std::string::size_type how_much) {
    dest->append(&read_buf[read_processed], how_much);
    read_processed += how_much;
    cm_processed += how_much;
  }

/*
  // Number of 'segment' in which is the program. This is chyba useless
  // variable now.
  int which_segment;
*/

  // // Queue of errors that shall be handled somehow.
  // std::queue<Error> errors;
};

}  // namespace tin

#endif  // SERVER_APP_SOCKET_STUFF_H_
