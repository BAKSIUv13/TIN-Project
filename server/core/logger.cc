// Copyright 2019 Piotrek

#include "core/logger.h"

namespace tin {
  std::string Logger::GenerateTime_() {
    return "[siódma rano]\n";
  }

  Logger Logger::LogH(Logger::DefaultLogFn_<LogLevel::HIGH>);
  Logger Logger::LogM(Logger::DefaultLogFn_<LogLevel::MEDIUM>);
  Logger Logger::LogL(Logger::DefaultLogFn_<LogLevel::LOW>);
  Logger Logger::LogVL(Logger::DefaultLogFn_<LogLevel::VERY_LOW>);

  Logger &LogH = Logger::LogH;
  Logger &LogM = Logger::LogM;
  Logger &LogL = Logger::LogL;
  Logger &LogVL = Logger::LogVL;
}  // namespace tin
