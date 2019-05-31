// Copyright 2019 Piotrek

#include "core/logger.h"

#include <ctime>
#include <iomanip>

namespace tin {
  std::string Logger::GenerateTime_() {
    std::time_t t = std::time(nullptr);
    std::stringstream s;
    s << std::put_time(std::localtime(&t), "[%F %T %Z]%n");
    return s.str();
  }

  std::ofstream Logger::default_log_file_
    (Logger::log_file_name_, std::ios::out);

  Logger Logger::LogH(Logger::DefaultLogFn_<LogLevel::HIGH>);
  Logger Logger::LogM(Logger::DefaultLogFn_<LogLevel::MEDIUM>);
  Logger Logger::LogL(Logger::DefaultLogFn_<LogLevel::LOW>);
  Logger Logger::LogVL(Logger::DefaultLogFn_<LogLevel::VERY_LOW>);

  Logger &LogH = Logger::LogH;
  Logger &LogM = Logger::LogM;
  Logger &LogL = Logger::LogL;
  Logger &LogVL = Logger::LogVL;
}  // namespace tin
