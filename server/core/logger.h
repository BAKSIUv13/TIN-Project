// Copyright 2019 TIN

#ifndef SERVER_CORE_LOGGER_H_
#define SERVER_CORE_LOGGER_H_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <functional>


namespace tin {

enum class LogLevel {
  VERY_LOW,
  LOW,
  MEDIUM,
  HIGH,
};

class Logger : public std::ostream {
 public:
  using Fn = std::function<void(const std::ostringstream &)>;

  class Internal : public std::ostream {
    friend class Logger;
   public:
    ~Internal() = default;
    template <typename T>
    Internal &operator<<(const T &arg) {
      ss_ << arg;
      fn_(ss_);
      ss_.str("");
      return *this;
    }

   private:
    explicit Internal(const Fn &fn) : ss_(), fn_(fn) {}
    Internal(const Internal &) = default;

    std::ostringstream ss_;
    Fn fn_;
  };

  explicit Logger(Fn fn)
      : internal_(fn) {}
  Logger(const Logger &) = default;
  ~Logger() = default;
  template <typename T>
  Internal &operator<<(const T &arg) {
    internal_ << GenerateTime_();
    return internal_ << arg;
  }

  static Logger LogH, LogM, LogL, LogVL;

 private:
#ifdef NDEBUG
  static constexpr bool DEBUG = false;
#else
  static constexpr bool DEBUG = true;
#endif
  static std::string GenerateTime_();
  static constexpr const char *log_file_name_ = "./log.log";
  static std::ofstream default_log_file_;
  template <LogLevel LvL>
  static void DefaultLogFn_(const std::ostringstream &oss) {
    if ((DEBUG && LvL >= LogLevel::LOW) || LvL >= LogLevel::HIGH) {
      std::cerr << oss.str();
    }
    if (DEBUG || LvL >= LogLevel::MEDIUM) {
      if (default_log_file_.is_open()) {
        default_log_file_ << oss.str();
      }
    }
  }
  Internal internal_;
};

extern Logger \
&LogH, &LogM, &LogL, &LogVL;

}  // namespace tin

#endif  // SERVER_CORE_LOGGER_H_
