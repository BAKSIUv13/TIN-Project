// Copyright 2077 jabka

#ifndef SERVER_CONFIGURATOR_THE_CONFIG_H_
#define SERVER_CONFIGURATOR_THE_CONFIG_H_

#include <string>

#include "configurator/setting.h"

namespace tin {
class TheConfig {
 public:
  TheConfig() {}
  ~TheConfig() {}

  std::string GetSetting(Setting) {return std::string();}
  void SetSetting(Setting, std::string) {}

  void LoadFromFile(std::string path) {}
  void SaveToFile(std::string path) {}

 private:
  void LoadDefault_() {}
};  // class TheConfig
}  // namespace tin

#endif  // SERVER_CONFIGURATOR_THE_CONFIG_H_
