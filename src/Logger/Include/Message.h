#pragma once

#include <array>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>
#include <utility>

template <std::size_t N>
class Message {
public:
  ~Message() = default;

  template <typename... Args>
  Message(const char* fmt, Args&&... args) noexcept {
    // Ensure we pass the correct buffer size (N) and handle snprintf return values.
    size_t written = 0;
    if constexpr (sizeof...(Args) == 0) {
      written = std::snprintf(mBuffer.data(), mBuffer.size(), "%s", fmt);
    } else {
      written = std::snprintf(mBuffer.data(), mBuffer.size(), fmt, std::forward<Args>(args)...);
    }

    if (written < 0) {
      // encoding/other error
      if (!mBuffer.empty()) {
        mBuffer[0] = '\0';
      }
    } else if (static_cast<std::size_t>(written) >= mBuffer.size()) {
      // truncated; snprintf guarantees null-termination when size>0.
      // Add "..." to indicate truncation if there's enough space.
      if (mBuffer.size() > 3) {
        std::strcpy(&mBuffer[mBuffer.size() - 4], "...");
      }
    }
  }

  // This class holds a buffer and can be expensive to copy.
  // To prevent accidental copies, we delete the copy operations.
  Message(const Message&) = delete;
  Message& operator=(const Message&) = delete;
  Message(Message&&) = default;
  Message& operator=(Message&&) = default;

  std::string_view View() const noexcept { return std::string_view(mBuffer.data()); }

  // Conversion operator to std::string for cases requiring ownership (like exceptions).
  operator std::string() const { return std::string(mBuffer.data()); }

private:
  std::array<char, N> mBuffer{};
};