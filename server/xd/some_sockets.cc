// Copyright 2077 jabka

#include "xd/some_sockets.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <cstring>
#include <iostream>

#include "core/thread.h"

namespace tin {
  using std::size_t;

  void pisanko(int fd) {
    for (int i = 0; i < 10; ++i) {
      sleep(1);
      dprintf(fd, "xdxdxd %d %d\n", 666, i);
    }
    dprintf(fd, "koniec\n");
  }

  void czytanko(int fd) {
    static constexpr std::size_t BUFF_LEN = 256;
    while (true) {
      char buff[BUFF_LEN + 1];
      ssize_t len;
      len = read(fd, buff, BUFF_LEN);
      if (len == 0) {
        return;
      }
      buff[len] = '\0';
      std::cout << "Czytanko: " << buff;
      if (buff[len - 1] != '\n') {
        std::cout << '\n';
      }
      if (std::strcmp(buff, "koniec\n") == 0) {
        return;
      }
    }
  }

  void *server_func(void *vp) {
    int serv_sock;
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock < 0) {
      std::cerr << "server socket creation failed: " << serv_sock << ", "
        << errno << '\n';
      return nullptr;
    }
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(33000);
    int bind_ret;
    bind_ret = bind(serv_sock,
      reinterpret_cast<const struct sockaddr *>(&saddr),
      // (struct sockaddr *)&saddr,
      sizeof(saddr));
    if (bind_ret != 0) {
      std::cerr << "bind error: " << bind_ret << '\n'
        << "errno: " << std::strerror(errno) << '\n';
      return nullptr;
    }
    int listen_ret;
    listen_ret = listen(serv_sock, 16);
    if (listen_ret != 0) {
      std::cerr << "listen error: " << listen_ret << '\n'
        << "errno: " << std::strerror(errno) << '\n';
      return nullptr;
    }
    struct sockaddr cliaddr;
    int connection_socket;
    socklen_t addr_size;
    connection_socket = accept(serv_sock, &cliaddr, &addr_size);
    if (listen_ret != 0) {
      std::cerr << "accept error: " << connection_socket << '\n'
        << "errno: " << std::strerror(errno) << '\n';
      return nullptr;
    }
    std::cout << "Serwer się połączył z klientem xd\n";
    czytanko(connection_socket);
    close(connection_socket);
    close(serv_sock);
    return nullptr;
  }

  void *client_func(void *vp) {
    sleep(3);
    int cli_sock;
    cli_sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    int connect_ret;
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_LOOPBACK;
    saddr.sin_port = htons(33000);
    connect_ret = connect(cli_sock,
      reinterpret_cast<const struct sockaddr *>(&saddr),
      sizeof(saddr));
    if (connect_ret != 0) {
      std::cerr << "connect error: " << connect_ret << '\n'
        << "errno: " << std::strerror(errno) << '\n';
      return nullptr;
    }
    std::cout << "Połączyło się z serwerem :3\n";
    pisanko(cli_sock);
    close(cli_sock);
    return nullptr;
  }

  int func(int argc, char **argv, char **env) {
    // while (fork()) {}
    /*
    std::cout << "args: " << '\n';
    for (int i = 0; i < argc ; ++i) {
      std::cout << argv[i] << ' ';
    }
    std::cout << "\nenv:" << '\n';
    for (auto it = env; it != nullptr; ++it) {
      std::cout << *it <<'\n';
    }
    */
    Thread client_thr, serv_thr;
    serv_thr = Thread(server_func, nullptr);
    client_thr = Thread(client_func, nullptr);
    serv_thr.Join();
    client_thr.Join();
    return 0;
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
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(33000);
    int bind_ret;
    bind_ret = bind(socks[0],
      reinterpret_cast<const struct sockaddr *>(&saddr),
      // (struct sockaddr *)&saddr,
      sizeof(saddr));
    if (bind_ret != 0) {
      std::cerr << "bind error: " << bind_ret << '\n'
        << "errno: " << std::strerror(errno) << '\n';
      return 2;
    }
    int listen_ret;
    listen_ret = listen(socks[0], 16);
    if (listen_ret != 0) {
      std::cerr << "listen error: " << listen_ret << '\n'
        << "errno: " << std::strerror(errno) << '\n';
      return 2;
    }
    struct sockaddr cliaddr;
    int accept_ret;
    socklen_t addr_size;
    accept_ret = accept(socks[0], &cliaddr, &addr_size);
    if (listen_ret != 0) {
      std::cerr << "accept error: " << accept_ret << '\n'
        << "errno: " << std::strerror(errno) << '\n';
      return 2;
    }
    return 0;
  }
}  // namespace tin
