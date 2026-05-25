#include "Logger.h"
#include <chrono>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace Components::Thread;
namespace Components::Logger {
namespace {
constexpr std::string_view CategoryConvert(Category category) noexcept {
  switch (category) {
  case Category::Init:
    return "Init";
  case Category::Thread:
    return "Thread";
  case Category::Download:
    return "Download";
  case Category::Processing:
    return "Processing";
  case Category::Network:
    return "Network";
  default:
    return "Unknown";
  }
}

constexpr std::size_t LongestCategoryLength() noexcept {
  std::size_t maxLength = 0;
  for (uint8_t i = 0; i < static_cast<uint8_t>(Category::COUNT); ++i) {
    auto length = CategoryConvert(static_cast<Category>(i)).size();
    if (length > maxLength) {
      maxLength = length;
    }
  }
  return maxLength;
}

constexpr std::string_view LevelConvert(Level level) noexcept {
  switch (level) {
  case Level::Info:
    return "\033[32mINF\033[0m";
  case Level::Warn:
    return "\033[33mWRN\033[0m";
  case Level::Error:
    return "\033[31mERR\033[0m";
  case Level::Trace:
    return "\033[34mTRC\033[0m";
  default:
    return "\033[31mUNK\033[0m";
  }
}

std::string_view FormatTime(std::array<char, kMaxBufferSize>& buffer, const std::chrono::system_clock::time_point& tp) noexcept {
  using namespace std::chrono;

  auto seconds_epoch = floor<seconds>(tp);
  auto millis = duration_cast<milliseconds>(tp - seconds_epoch).count();

  std::time_t t = system_clock::to_time_t(tp);
  std::tm tm;
  localtime_r(&t, &tm);

  int len = std::snprintf(buffer.data(), buffer.size(), "%02d:%02d:%02d.%03lld", tm.tm_hour, tm.tm_min, tm.tm_sec, (long long)millis);

  return std::string_view(buffer.data(), (len > 0 && (size_t)len < buffer.size()) ? len : 0);
}
} // namespace

LoggerThread::LoggerThread() : Thread("LoggerThread") {}

LoggerThread::~LoggerThread() {
  cv.notify_one();
  Stop();
}

void LoggerThread::PopLog() noexcept {
  auto entry = std::move(mQueue.front());
  mQueue.pop();

  Log(entry.meta.level, entry.meta.category, entry.meta.tp, entry.msg.View());
}

void LoggerThread::Exec() {
  while (true) {
    std::unique_lock lock(mMutex);
    // Wait until there is a message OR the thread is stopped
    cv.wait(lock, [this]() { return !mQueue.empty() || !IsRunning(); });

    // Exit only if the thread is stopped and no messages are left
    if (mQueue.empty() && !IsRunning()) {
      break;
    }

    if (!mQueue.empty()) {
      PopLog();
    }
  }
}

void LoggerThread::Receive(LogEntry&& entry) noexcept {
  std::unique_lock lock(mMutex);
  mQueue.push(std::move(entry));

  // notify thread
  cv.notify_one();
}

void LoggerThread::Log(Level level, Category category, std::chrono::system_clock::time_point tp, const std::string_view msg) noexcept {
  auto timeStr = FormatTime(mTimeBuffer, tp);
  auto levelStr = LevelConvert(level);
  auto categoryStr = CategoryConvert(category);

  std::cout << "[" << timeStr << "][" << levelStr << "][" << std::setw(LongestCategoryLength()) << std::left << std::setfill(' ')
            << categoryStr << "] " << msg << std::endl;
}

Logger::Logger() { mLoggerThread.Start(); }

void Logger::Info(Category category, LogMessage&& msg) noexcept {
  mLoggerThread.Receive(
      LogEntry{.meta = LogMeta{.level = Level::Info, .category = category, .tp = std::chrono::system_clock::now()}, .msg = std::move(msg)});
}

void Logger::Warn(Category category, LogMessage&& msg) noexcept {
  mLoggerThread.Receive(
      LogEntry{.meta = LogMeta{.level = Level::Warn, .category = category, .tp = std::chrono::system_clock::now()}, .msg = std::move(msg)});
}

void Logger::Error(Category category, LogMessage&& msg) noexcept {
  mLoggerThread.Receive(LogEntry{.meta = LogMeta{.level = Level::Error, .category = category, .tp = std::chrono::system_clock::now()},
                                 .msg = std::move(msg)});
}

void Logger::Trace(Category category, LogMessage&& msg) noexcept {
  mLoggerThread.Receive(LogEntry{.meta = LogMeta{.level = Level::Trace, .category = category, .tp = std::chrono::system_clock::now()},
                                 .msg = std::move(msg)});
}
} // namespace Components::Logger
