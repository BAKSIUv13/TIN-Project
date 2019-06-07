// Copyright 2019 TIN

#include <arpa/inet.h>
#include <signal.h>

#include <iostream>
#include <array>
#include <string>
#include <cstdint>
#include <cinttypes>

#include "core/server.h"
#include "core/username.h"
#include "core/nquad.h"
#include "core/mquads.h"
#include "core/logger.h"

#include "image/utility.h"

static tin::Server *interrupt_server_ptr;

static void interrupt(int) {
  if (interrupt_server_ptr != nullptr) {
    tin::LogH << "SIGINT\n";
    interrupt_server_ptr->StopRun();
  }
  interrupt_server_ptr = nullptr;
}


int main(int argc, char **argv, char **env) {
  tin::LogH << "Uruchamianie serwera.\n";

  // return tin::AccountManager::test(argc, argv, env);

  uint16_t port = 0;
  bool arg_port = false;
  if (argc > 1) {
    sscanf(argv[1], "%" SCNd16, &port);
    arg_port = true;
  }


  tin::Vec2 v;
  tin::Transform t;
  auto x = tin::Utility::translate(t, v);
  x = x;

  struct sigaction sa;
  struct sigaction old_sa;
  memset(&sa, '\0', sizeof(sa));
  sa.sa_handler = &interrupt;
  sigfillset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_restorer = nullptr;
  tin::Server server;
  interrupt_server_ptr = &server;
  sigaction(SIGINT, &sa, &old_sa);

  {
    std::fstream f("shadow.log", std::ios::in);
    if (!f.is_open()) {
      f.open("shadow.log", std::ios::out);
      std::fstream f2("shadow.default", std::ios::in);
      while (true) {
        int c = f2.get();
        if (!f2.good()) break;
        f.put(c);
      }
    }
  }

  server.SpecialHardcodeInit();
  // server.AttachAccountFile("shadow.log", false);
  arg_port ? server.Run(port) : server.Run();
  sigaction(SIGINT, &old_sa, nullptr);
}
