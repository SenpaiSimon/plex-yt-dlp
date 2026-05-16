#pragma once

#include "Message.h"
#include <cstdint>
#include <string_view>

namespace Components::Logger {
enum class Category : uint8_t {
  Init,
  Download,
  Processing,
  Network,
  Thread,
  COUNT,
};
enum class Level : uint8_t { Info, Warn, Error, Trace };

using LogMessage = Message<256>;

class ILogger {
public:
  virtual ~ILogger() = default;
  // Accept a pre-formatted message (use `std::format` at call-site).
  virtual void Info(Category category, const LogMessage&& msg) noexcept = 0;
  virtual void Warn(Category category, const LogMessage&& msg) noexcept = 0;
  virtual void Error(Category category, const LogMessage&& msg) noexcept = 0;
  virtual void Trace(Category category, const LogMessage&& msg) noexcept = 0;
};
} // namespace Components::Logger