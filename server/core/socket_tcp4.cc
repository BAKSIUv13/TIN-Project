// Copyright 1410 Jurand ze Spychowa

#include "core/socket_tcp4.h"

#define SOME_DEBUG

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <cstring>
#include <iostream>
#include <array>

static std::array<char, 16> fgfg(uint32_t x) {
  // nie chciało mi się szukać tyhc funkcji więc napisałem sam na rzaie
  std::array<char, 16> xc;
  char *gd = xc.data();
  for (uint32_t i = 24;; i -= 8) {
    gd += snprintf(gd, 16 * sizeof(char), "%d.", (x >> i) & 255);
    if (i == 0) break;
  }
  *(gd - 1) = '\0';
  return xc;
}

namespace tin {
  SocketTCP4::SocketTCP4()
      : fd_(-1), status_(BLANK) {
    std::cerr << "SocketTCP4: empty constructor\n";
  }  // fd -1 means socket is not open

  //  SocketTCP4::SocketTCP4(uint32_t address, uint16_t port)
  //  : Socket() {
  //  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  SocketTCP4::~SocketTCP4() {
    std::cerr << "SocketTCP4: destructor\n";
    Destroy_();
  }

  int SocketTCP4::Open() {
    std::cerr << "SocketTCP4: open\n";
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
#ifdef SOME_DEBUG
      std::cerr << "Could not create socket :<\n";
#endif
      return -1;
    }
    fd_ = sock_fd;
    status_ = CREATED;
    return 0;
  }

  SocketTCP4::SocketTCP4(SocketTCP4 &&other) noexcept {
    std::cerr << "SocketTCP4: move constructor\n";
    Move_(&other);
  }

  SocketTCP4 &SocketTCP4::operator=(SocketTCP4 &&other) noexcept {
    std::cerr << "SocketTCP4: move assign\n";
    if (this == &other) {
      return *this;
    }
    Destroy_();
    Move_(&other);
    return *this;
  }

  void SocketTCP4::Destroy_() {
    std::cerr << "SocketTCP4: destroy\n";
    if (status_ != BLANK) {
      close(fd_);
      status_ = BLANK;
    }
  }

  void SocketTCP4::Move_(SocketTCP4 *other) {
    std::cerr << "SocketTCP4: move\n";
    std::memcpy(this, other, sizeof(*this));
    other->status_ = BLANK;
  }

  int SocketTCP4::Bind(uint32_t address, uint16_t port) {
    std::cerr << "SocketTCP4: bind\n";
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(address);
    saddr.sin_port = htons(port);
    int bind_ret = bind(fd_,
      reinterpret_cast<struct sockaddr *>(&saddr),
      sizeof(saddr));
    if (bind_ret == 0) {
      addr_here_ = address;  // nie wiem
      port_here_ = port;  // czy dobrze xd
      status_ = BOND;
#ifdef SOME_DEBUG
    } else {
      std::cerr << "Bind nie poszedl :<\n";
#endif
    }
    return bind_ret;
  }

  int SocketTCP4::BindAny(uint16_t port) {
    std::cerr << "SocketTCP4: bind any\n";
    return Bind(INADDR_ANY, port);
  }

  int SocketTCP4::Listen(int queue_length) {
    std::cerr << "SocketTCP4: listen\n";
    int listen_ret = listen(fd_, queue_length);
    if (listen_ret == 0) {
      status_ = LISTENING;
    }
    return listen_ret;
  }

  int SocketTCP4::Connect(uint32_t address, uint16_t port) {
    std::cerr << "SocketTCP4: connect\n";
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(address);
    saddr.sin_port = htons(port);
    int connect_ret = connect(fd_,
      reinterpret_cast<struct sockaddr *>(&saddr),
      sizeof(saddr));
    if (connect_ret == 0) {
      addr_there_ = address;
      port_there_ = port;
      status_ = CONNECTED;
    }
    return connect_ret;
  }

  int SocketTCP4::Accept(SocketTCP4 *new_sock) {
    std::cerr << "SocketTCP4: accept\n";
    if (new_sock->status_ != BLANK) {
      return -1000;
    }
    struct sockaddr saddr_l;
    struct sockaddr_in &saddr
      = *reinterpret_cast<struct sockaddr_in*>(&saddr_l);
    socklen_t addrlen = sizeof(saddr);
    int accept_ret = accept(fd_, &saddr_l, &addrlen);
    if (accept_ret >= 0) {
      if (addrlen != sizeof(saddr)) {
        std::cerr << "xd\n";
        std::terminate();
      }
      new_sock->status_ = CONNECTED;
      new_sock->fd_ = accept_ret;
      new_sock->addr_there_ = ntohl(saddr.sin_addr.s_addr);
      new_sock->port_there_ = ntohs(saddr.sin_port);
      int hahaha = getsockname(accept_ret, &saddr_l, &addrlen);
      if (hahaha < 0) {
        std::cerr << "najgorzej\n";
        std::terminate();
      }
      new_sock->addr_here_ = ntohl(saddr.sin_addr.s_addr);
      new_sock->port_here_ = ntohs(saddr.sin_port);
      std::fprintf(stderr, "serv here xd  %s:%d\nclient here   %s:%d\n"
        "client there  %s:%d\n",
        fgfg(addr_here_).data(), port_here_,
        fgfg(new_sock->addr_here_).data(), new_sock->port_here_,
        fgfg(new_sock->addr_there_).data(), new_sock->port_there_);
      return 0;
    }
    return accept_ret;
  }

  int SocketTCP4::Close() {
    std::cerr << "SocketTCP4: close\n";
    int close_ret = close(fd_);
    status_ = BLANK;
    return close_ret;
  }

  int SocketTCP4::Shutdown(int how) {
    std::cerr << "SocketTCP4: shutdown\n";
    return shutdown(fd_, how);
  }
}  // namespace tin
