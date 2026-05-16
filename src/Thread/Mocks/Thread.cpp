#include "Thread.h"

#include <stdexcept>
#include <utility>

namespace Components::Thread {
Thread::Thread(std::string_view name, ThreadPriority priority, uint8_t coreId, uint32_t stackSize)
    : mName{name}, mPriority{priority}, mCoreId{coreId}, mStackSize{stackSize} {}

Thread::~Thread() {
  if (mRunning) {
    Stop();
  }
}

void Thread::Start() {
  mRunning = true;
  mThread = std::thread([this]() { Exec(); });
}

void Thread::Stop() {
  if (!mRunning) {
    throw std::runtime_error("Thread is not running");
  }

  mRunning = false;
  mThread.join();
}

bool Thread::IsRunning() const noexcept { return mRunning; }

std::string_view Thread::GetName() const noexcept { return mName; }
} // namespace Components::Thread