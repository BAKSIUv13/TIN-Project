// Copyright 2019 Piotrek

#include "core/logger.h"

namespace tin {
  Logger Logger::LogH(Logger::DefaultLogFn_<LogLevel::HIGH>);
  Logger Logger::LogM(Logger::DefaultLogFn_<LogLevel::MEDIUM>);
  Logger Logger::LogL(Logger::DefaultLogFn_<LogLevel::LOW>);

  Logger &LogH = Logger::LogH;
  Logger &LogM = Logger::LogM;
  Logger &LogL = Logger::LogL;
}  // namespace tin
