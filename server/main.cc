// Copyright 1077 jabka

#include <iostream>

#include "xd/some_sockets.h"
#include "app/server.h"

int main(int argc, char **argv, char **env) {
  //  return tin::func(argc, argv, env);
  char x[4] = {'O', 'w', 'O', '!'};
  int &xx = *reinterpret_cast<int *>(&x);
  std::cout << xx << '\n';
  tin::Server server;
  server.Run();
}
