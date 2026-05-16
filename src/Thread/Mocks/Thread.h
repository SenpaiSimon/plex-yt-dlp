#pragma once

#include "IThread.h"
#include <thread>

namespace Components::Thread {
class Thread : public IThread {
public:
  ~Thread();

  Thread(std::string_view name, ThreadPriority priority = ThreadPriority::k5, uint8_t coreId = 0,
         uint32_t stackSize = 4096);

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
  bool mRunning{false};
  std::thread mThread;
  std::string_view mName;
  ThreadPriority mPriority{};
  uint8_t mCoreId{};
  uint32_t mStackSize{};
};
} // namespace Components::Thread