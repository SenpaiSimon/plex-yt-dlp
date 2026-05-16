#pragma once

#include "IThread.h"
#include <atomic>
#include <string>
#include <thread>

namespace Components::Thread {
class Thread : public IThread {
public:
  ~Thread();

  Thread(std::string_view name);

  // delete copy and move constructors and assignment operators
  Thread(const Thread&) = delete;
  Thread& operator=(const Thread&) = delete;
  Thread(Thread&&) = delete;
  Thread& operator=(Thread&&) = delete;

  void Start();
  void Stop() final;
  bool IsRunning() const noexcept final;
  std::string_view GetName() const noexcept final;

  virtual void Exec() = 0;

private:
  std::atomic<bool> mRunning{false};
  std::thread mThread;
  std::string mName;
};
} // namespace Components::Thread