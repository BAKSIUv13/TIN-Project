// Copyright 0 0

#include "network/network_manager.h"

#include <unistd.h>

#include <utility>
#include <iostream>
#include <string>

namespace tin {
  int NetworkManager::Initialize(const NetStartConf &conf) {
    int result = 0;
    result = sock.Open();
    if (result < 0) return -1;
    result = sock.BindAny(conf.port);
    if (result < 0) return -2;
    result = sock.Listen(DEFAULT_QUEUE_LEN);
    if (result < 0) return -3;
    return 0;
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
    for (auto it = nws->messages_to_send.begin();
        it != nws->messages_to_send.end(); ++it) {
      int fd = it->first;
      if (client_socks.count(fd) < 1) {
        std::cerr << "W klienckich socketach nie ma socketu nr " << fd
          << ", a jest na liście rzeczy do wysłania! To źle!!!\n";
      } else {
        sel->AddFD(fd, Sel::WRITE);
      }
    }
  }

  void NetworkManager::DealWithSelResult(Sel *sel, Nws *nws) {
    static constexpr size_t BUF_SIZE = 256;
    static constexpr size_t MAX_SEND_SIZE = 256;
    char buf[BUF_SIZE + 1];
    if (Sel::READ & sel->Get(sock.GetFD())) {
      // Mamy nowe połączenie
      SocketTCP4 new_client;
      sock.Accept(&new_client);
      int fd = new_client.GetFD();
      client_socks.emplace(fd, std::move(new_client));
      std::cerr << "Serwer dostał nowe połączenie i zrobił nowe gniazdo "
        << fd << " teraz czekamy aż się zidentyfikuje chyba.\n";
      nws->AddSockToReg(fd);
    }
    for (
        auto it = nws->messages_to_send.begin();
        it != nws->messages_to_send.end();
        ++it) {
      int fd = it->first;
      std::string &s = it->second;
      if (Sel::WRITE & sel->Get(fd)) {
        size_t chars_to_send = s.size();
        if (chars_to_send > MAX_SEND_SIZE) {
          chars_to_send = MAX_SEND_SIZE;
        }
        int c = write(fd, s.c_str(), chars_to_send);
        if (c < 0) {
          std::cerr << "Błąd - nie udało się pisać do gniazda nr "
            << fd << " - kończę program :<\n";
          std::terminate();
        } else if (c == 0) {
          std::cerr << "Yyyyy nie wiem?? " << fd << '\n';
          std::terminate();
        } else {
          std::cerr << "Wysałno " << c << " znaków na gniazdo " << fd
            << '\n';
          decltype(s.size()) cs = c;
          if (cs == s.size()) {
            nws->messages_to_send.erase(it);
          } else if (cs > s.size()) {
            std::cerr << "To się chyba nie powinno nigdy zdarzyć?? xd lol\n"
              << "Gniazdo: " << fd
              << "\nRozmiar napisu w 'kolejce' do wysłania: " << s.size()
              << "\nIlość wysłanych znaków: " << c
              << '\n';
            std::terminate();
          } else {
            // tutaj już mamy dobrze xd
            std::cerr << "Nie udało się wysłać wszystkiego naraz, ale to nic\n"
              << "Gniazdo: " << fd
              << "\nRozmiar napisu w 'kolejce' do wysłania: " << s.size()
              << "\nIlość wysłanych znaków: " << c
              << '\n';
            s = s.substr(c);
          }
        }
      }
    }
    for (auto it = nws->sockets_to_close.begin();
        it != nws->sockets_to_close.end();
        ++it) {
      std::cerr << "Zamykam gniazdo nr " << *it << "\n";
      client_socks.erase(*it);
    }
    nws->sockets_to_close.clear();

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
          std::cerr << "Udaję, że je zamykam xd xd xd\n";
          nws->AddSockToDrop(fd);
        } else {
          buf[c] = '\0';
          std::cerr << "Gniazdo klienckie nr " << fd << " przysłało coś\n";
          std::cerr << buf;
          if (buf[c - 1] == '\n') {
            std::cerr << "\\n\n";
          } else {
            std::cerr << "\n--!\\n--\n";
          }
          nws->received_messages.push_back(
            {fd, std::string(buf)});
        }  // if if else
      }  // if
    }  // for
  }
// okropne
// void NetworkManager::CloseSock(int fd) {
//  client_socks.erase(fd);
// }

}  // namespace tin
