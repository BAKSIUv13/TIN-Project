// Copyright 2077 jabka

#include "xd/some_sockets.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <iostream>

namespace tin {
  int func(int argc, char **argv, char **env) {
    // while (fork()) {}
    int socks[3];
    socks[0] = socket(AF_INET, SOCK_STREAM, 0);
    socks[1] = socket(AF_INET, SOCK_STREAM, 0);
    socks[2] = socket(AF_INET, SOCK_STREAM, 0);
    if (socks[0] < 0 || socks[1] < 0 || socks[2] < 0) {
      return 1;
    }
    std::cout << "numery socketów: " << socks[0] << ", " << socks[1] \
      << ", " << socks[2] << "\n";
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_LOOPBACK;
    saddr.sin_port = htons(12341);
    int bind_ret = bind(socks[0],
      reinterpret_cast<const struct sockaddr *>(&saddr),
      sizeof(saddr));
    if (bind_ret != 0) {
      std::cerr << "bind error: " << bind_ret << '\n';
      return 2;
    }
    return 0;
  }
}  // namespace tin
