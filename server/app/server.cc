// Copyright 2077 jabka

#include "app/server.h"

#include <unistd.h>

#include <set>
#include <vector>
#include <iostream>

#include "core/socket_tcp4.h"

namespace tin {
Server::Server() {
  return;
}

void Server::Run() {
  static constexpr uint32_t HARDCODED_SERVER_ADDRESS = 0x7f000001;
  static constexpr uint16_t HARDCODED_SERVER_PORT = 23456;
  static constexpr size_t MAX_CLIENTS = 10;

  SocketTCP4 sock;

  char znaks[257];

  sock.Open();
  sock.BindAny(HARDCODED_SERVER_PORT);
  sock.Listen(16);

  std::vector<SocketTCP4> socks;

  int p1[2];

  pipe(p1);

  int fg = sock.GetFD();
  if (p1[0] > sock.GetFD()) {
    fg = p1[0];
  }

  int maxfd;

  //  struct timeval timeout;
  //  timeout.tv_sec = 10;

  fd_set set2;


  while (1) {
    maxfd = sock.GetFD();
    FD_ZERO(&set2);
    FD_SET(p1[0], &set2);
    for (auto &x : socks) {
      if (x.GetFD() > maxfd) maxfd = x.GetFD();
      FD_SET(x.GetFD(), &set2);
    }

    if (maxfd > FD_SETSIZE) {
      std::cerr << "xdxdxd\n";
      std::terminate();
    }

    int sss = select(maxfd, &set2, nullptr, nullptr, nullptr);

    if (sss == -1) {
      std::cerr << "kurde\n";
      std::terminate();
    } else if (sss == 0) {
      std::cerr << "jak? lel\n";
      std::terminate();
    } else {
      for (auto &x : socks) {
        if (FD_ISSET(x.GetFD(), &set2)) {
          std::cout << "Przyszło coś z " << x.GetFD() << ":\n";
          ssize_t ile = read(x.GetFD(),
            reinterpret_cast<void *>(znaks), sizeof(znaks));
        }
      }
      if (FD_ISSET(sock.GetFD(), &set2)) {
        socks.emplace_back(SocketTCP4());
        sock.Accept(&socks.back());
      }
      if (FD_ISSET(p1[0], &set2)) {
        break;
      }
    }
  }
}
}  // namespace tin
