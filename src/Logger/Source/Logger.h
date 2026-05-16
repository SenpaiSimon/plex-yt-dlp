#pragma once

#include "ILogger.h"
#include "Thread.h"
#include <array>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace Components::Logger {

constexpr uint32_t kMaxBufferSize = 32;

struct LogMeta {
  Level level;
  Category category;
  std::chrono::system_clock::time_point tp;
};

struct LogEntry {
  const LogMeta meta;
  const LogMessage&& msg;
};

class LoggerThread : public Thread::Thread {
public:
  LoggerThread();
  ~LoggerThread();

  void Exec() final;

  void Receive(LogEntry&& entry) noexcept;

private:
  void Log(Level level, Category category, const std::string_view msg) noexcept;
  // semaphore for queue
  std::mutex mMutex;
  std::condition_variable cv;
  std::queue<LogEntry> mQueue{};
  std::array<char, kMaxBufferSize> mTimeBuffer{};
};

class Logger : public ILogger {
public:
  Logger();

  void Info(Category category, const LogMessage&& msg) noexcept final;
  void Warn(Category category, const LogMessage&& msg) noexcept final;
  void Error(Category category, const LogMessage&& msg) noexcept final;
  void Trace(Category category, const LogMessage&& msg) noexcept final;

private:
  LoggerThread mLoggerThread;
};
} // namespace Components::Logger