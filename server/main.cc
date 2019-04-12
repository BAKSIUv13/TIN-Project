// Copyright 1077 jabka

#include <iostream>
#include <array>

#include <arpa/inet.h>

#include "xd/some_sockets.h"
#include "app/server.h"

/*
static std::array<char, 16> fgfg(uint32_t x) {
  // nie chciało mi się szukać tyhc funkcji więc napisałem sam na rzaie
  std::array<char, 16> xc;
  char *gd = xc.data();
  for (uint32_t i = 0; i < 32; i += 8) {
    gd += snprintf(gd, 16 * sizeof(char), "%d.", (x >> i) & 255);
  }
  *(gd - 1) = '\0';
  return xc;
}*/

int main(int argc, char **argv, char **env) {
  //  return tin::func(argc, argv, env);
  char x[4] = {'O', 'w', 'O', '!'};
  int &xx = *reinterpret_cast<int *>(&x);
  std::cout << xx << '\n';

  //  std::cout << fgfg(htonl(0x7f000001)).data() << '\n';
  //  std::cout << fgfg(htonl(0xc0a80164)).data() << '\n';
  //  std::cout << fgfg(htonl(0xffffffff)).data() << '\n';

  //  return 0;

  tin::Server server;
  server.Run();
}
