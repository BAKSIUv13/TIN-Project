// Copyright 1410 Jurand ze Spychowa

#include "core/socket.h"

#define SOME_DEBUG

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <cstring>
#include <iostream>


namespace tin {
  SocketTCP4::SocketTCP4()
    : status_(BLANK), fd_(-1) {
  }  // fd -1 means socket is not open

  //  SocketTCP4::SocketTCP4(uint32_t address, uint16_t port)
  //  : Socket() {
  //  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  int SocketTCP4::Create() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
#ifdef SOME_DEBUG
      std::cerr << "Could not create socket :<\n";
#endif
      return -1;
    }
    fd_ = sock_fd;
    status_ = CREATED;
  }

  int SocketTCP4::Bind(uint32_t address, uint16_t port) {
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(address);
    saddr.sin_port = htons(port);
    int bind_ret = bind(fd_,
      reinterpret_cast<struct sockaddr *>(&saddr),
      sizeof(saddr));
    if (bind_ret < 0) {
#ifdef SOME_DEBUG
      std::cerr << "Bind nie poszedl :<\n";
#endif
      return -1;
    }
    addr_local_ = address;  // nie wiem
    port_local_ = port;  // czy dobrze xd
    status_ = BOND;
    return 0;
  }

  int SocketTCP4::BindAny(uint16_t port) {
    return Bind(INADDR_ANY, port);
  }
}  // namespace tin
