// Copyright 0 0

#include "network/network_manager.h"

#include <unistd.h>

#include <utility>
#include <iostream>
#include <string>

namespace tin {
  void NetworkManager::Initialize(const NetStartConf &conf) {
    sock.Open();
    sock.BindAny(conf.port);
    sock.Listen(DEFAULT_QUEUE_LEN);
  }

  void NetworkManager::Finish() {
    sock.Close();
    std::terminate();
  }

  void NetworkManager::FeedMainSel(Sel *sel, Nws *nws) {
    sel->AddFD(sock.GetFD(), Sel::READ);
    for (auto it = client_socks.begin(); it != client_socks.end(); ++it) {
      sel->AddFD(it->second.GetFD(), Sel::READ);
    }
    // Tutaj będzie dodanie do selecta pisanko do socketów.
    (void)nws;
  }

  void NetworkManager::DealWithSelResult(Sel *sel, Nws *nws) {
    static constexpr size_t BUF_SIZE = 256;
    char buf[BUF_SIZE + 1];
    if (Sel::READ & sel->Get(sock.GetFD())) {
      // Mamy nowe połączenie
      SocketTCP4 new_client;
      sock.Accept(&new_client);
      int fd = new_client.GetFD();
      client_socks.emplace(fd, std::move(new_client));
      std::cerr << "Serwer dostał nowe połączenie i zrobił nowe gniazdo "
        << fd << "teraz czekamy aż się zidentyfikuje chyba.\n";
    }
    for (auto it = client_socks.begin(); it != client_socks.end(); ++it) {
      int fd = it->second.GetFD();
      if (Sel::READ & sel->Get(fd)) {
        ssize_t c = read(fd, buf, BUF_SIZE);
        if (c < 0) {
          std::cerr << "Błąd przy czytaniu z gniazda klienta nr " << fd
            << ":<\n";
          std::terminate();
        } else if (c == 0) {
          std::cerr << "Gniazdo klienckie nr " << fd << " przysłąło koniec\n";
          std::cerr << "Udaję, że je zamykam xd\n";
          std::terminate();
        } else {
          buf[c] = '\0';
          std::cerr << "Gniazdo klienckie nr " << fd << "przysłało coś\n";
          std::cerr << buf;
          if (buf[c - 1] == '\n') {
            std::cerr << "\\n\n";
          } else {
            std::cerr << "\n--!\\n--\n";
          }
          nws->received_messages.push_back(
            std::make_tuple(fd, std::string(buf)));
        }  // if if else
      }  // if
    }  // for
  }

}  // namespace tin
