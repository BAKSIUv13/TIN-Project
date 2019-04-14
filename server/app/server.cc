// Copyright 2077 jabka

#include "app/server.h"

#include <unistd.h>

#include <set>
#include <vector>
#include <iostream>
#include <cstring>

#include "core/socket_tcp4.h"

namespace tin {
Server::Server() {
  if (pipe(end_pipe_)) {
    std::cerr << "Serwer się nie zrobił bo pipe się  niwe otworzyło :< "
      << std::strerror(errno) << '\n';
    std::terminate();
  }
  return;
}
Server::~Server() {
  close(end_pipe_[0]);
  close(end_pipe_[1]);
}

void Server::Run2() {
  static constexpr NetStartConf HARDCODED_NET = {42000};
  static constexpr int STDIN_FD = STDIN_FILENO;
  nm_.Start(HARDCODED_NET);
  bool not_exit = true;
  Sel sel;
  while (not_exit) {
    if (true) {
      sel.AddFD(STDIN_FD, Sel::READ);
    }
    sel.AddFD(end_pipe_[0], Sel::READ);
    nm_.FeedMainSel(&sel);

    sel.Select();
    if (Sel::READ & sel.Get(end_pipe_[0])) {
      not_exit = false;
    } else if (Sel::READ & sel.Get(STDIN_FD)) {
      // Tutaj będzie obsługa stdin.
    } else {
      nm_.DealWithSelResult(&sel);
    }
  }
}

void Server::Run() {
  //  static constexpr uint32_t HARDCODED_SERVER_ADDRESS = 0x7f000001;
  static constexpr uint16_t HARDCODED_SERVER_PORT = 23456;
  //  static constexpr size_t MAX_CLIENTS = 10;

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
  //  fd_set setout;


  while (1) {
    maxfd = fg;
    FD_ZERO(&set2);
    //  FD_ZERO(&setout);
    FD_SET(0, &set2);
    FD_SET(p1[0], &set2);
    FD_SET(sock.GetFD(), &set2);
    for (auto it = socks.begin(); it < socks.end(); ++it) {
      auto &x = *it;
      if (x.GetStatus() != x.BLANK) {
        int fd = x.GetFD();
        if (fd > maxfd)
          maxfd = fd;
        FD_SET(fd, &set2);
      } else {
        auto iit = it - 1;
        socks.erase(it);
        it = iit;
      }
    }

    if (maxfd > FD_SETSIZE) {
      std::cerr << "xdxdxd\n";
      std::terminate();
    }

    std::cerr << "Ustawione już maski i tak dalej xd\n";

    int sss = select(maxfd + 1, &set2, nullptr, nullptr, nullptr);

    std::cerr << "select coś znalazł\n";

    if (sss == -1) {
      std::cerr << "kurde\n";
      std::terminate();
    } else if (sss == 0) {
      std::cerr << "jak? lel\n";
      std::terminate();
    } else {
      if (FD_ISSET(p1[0], &set2)) {
        std::cerr << "kończenie lel\n";
        break;
      } else if (FD_ISSET(sock.GetFD(), &set2)) {
        std::cerr << "select znalazl coś na słuchaniu \n";
        socks.emplace_back(SocketTCP4());
        sock.Accept(&socks.back());
        std::cerr << "Mam nowe połączonko " << socks.back().GetFD() << '\n';
      } else if (FD_ISSET(0, &set2)) {
        std::cerr << "coś na stdin\n";
        ssize_t ile = read(0,
          reinterpret_cast<void *>(znaks), sizeof(znaks));
        znaks[ile] = '\0';
        if (znaks[0] == 'x') {
          std::cerr << "kończenie z stdin xd\n";
          break;
        } else {
          int fd;
          if (sscanf(znaks, "%d", &fd) <= 0) {
            std::cerr << "nie było fd :<\n";
          } else {
            char *gd = znaks;
            //  bool wr = false;
            while (1) {
              if (*gd == '\0') {
                break;
              } else if (*gd == ':') {
                ++gd;
                write(fd, gd, strlen(gd));
                break;
              } else {
                ++gd;
              }
            }
          }
        }
      } else {
        std::cerr << "xdxdxdxd\n";
        for (auto &x : socks) {
          std::cerr << "Sprawdzam gniazdko " << x.GetFD() << '\n';
          if (FD_ISSET(x.GetFD(), &set2)) {
            std::cerr << "Przyszło coś z " << x.GetFD() << ":\n";
            ssize_t ile = read(x.GetFD(),
              reinterpret_cast<void *>(znaks), sizeof(znaks));
            if (ile == 0) {
              std::cerr << "Z gniazdka przyszło zamkncięcie (EOF) chyba czy coś"
                << ", więc zamykam\n";
              x.Close();
            } else {
              znaks[ile] = '\0';
              std::cerr << znaks;
              if (znaks[ile - 1] == '\n') {
                std::cerr << "\\n\n";
              } else {
                std::cerr << "\n--!\\n--\n";
              }
            }
            break;
          }
        }
      }
    }
  }
}
}  // namespace tin
