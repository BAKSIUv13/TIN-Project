// Copyright 2019 TIN

#include <arpa/inet.h>

#include <iostream>
#include <array>
#include <string>
#include <cstdint>
#include <cinttypes>

#include "app/server.h"
#include "core/username.h"
#include "core/nquad.h"
#include "core/mquads.h"
#include "core/logger.h"

void print_nquad(tin::NQuad nq) {
  char x[5];
  x[4] = '\0';
  memcpy(x, &nq, 4);
  std::ios ios_state(nullptr);
  ios_state.copyfmt(std::cerr);
  std::cerr << std::hex << nq << " : " << x << '\n';
  std::cerr.copyfmt(ios_state);
}

int main(int argc, char **argv, char **env) {

  tin::LogH << "Uruchamianie serwera.\n";

  uint16_t port = 0;
  bool arg_port = false;
  if (argc > 1) {
    sscanf(argv[1], "%" SCNd16, &port);
    arg_port = true;
  }


  tin::Server server;
  server.SpecialHardcodeInit();
  arg_port ? server.Run(port) : server.Run();
}
