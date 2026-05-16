#pragma once

#include <string_view>

namespace Components::Thread {
enum class ThreadPriority : int {
  k1 = 1,
  k2,
  k3,
  k4,
  k5,
  k6,
  k7,
  k8,
  k9,
  k10,
};

class IThread {
public:
  virtual ~IThread() = default;

  // Wait for the thread to finish execution.
  virtual void Stop() = 0;

  // Check if the thread is currently running.
  virtual bool IsRunning() const noexcept = 0;

  // Get your name
  virtual std::string_view GetName() const noexcept = 0;
};
} // namespace Components::Thread
