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

static tin::Server *interrupt_server_ptr;

static void interrupt(int) {
  tin::LogH << "SIGINT\n";
  if (interrupt_server_ptr != nullptr) {
    interrupt_server_ptr->StopRun();
  }
}

int main(int argc, char **argv, char **env) {
  tin::LogH << "Uruchamianie serwera.\n";

  uint16_t port = 0;
  bool arg_port = false;
  if (argc > 1) {
    sscanf(argv[1], "%" SCNd16, &port);
    arg_port = true;
  }

  struct sigaction sa;
  memset(&sa, '\0', sizeof(sa));
  sa.sa_handler = &interrupt;
  sigfillset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_restorer = nullptr;
  tin::Server server;
  interrupt_server_ptr = &server;
  sigaction(SIGINT, &sa, nullptr);

  server.SpecialHardcodeInit();
  arg_port ? server.Run(port) : server.Run();
}
