#include "Thread.h"

#include <stdexcept>
#include <utility>

namespace Components::Thread {
Thread::Thread(std::string_view name) : mName{name} {}

Thread::~Thread() { Stop(); }

void Thread::Start() {
  if (mRunning || mThread.joinable()) {
    throw std::runtime_error("Thread is already running");
  }

  mRunning = true;
  mThread = std::thread([this]() {
    Exec();
    mRunning = false;
  });
}

void Thread::Stop() {
  mRunning = false;
  if (mThread.joinable()) {
    mThread.join();
  }
}

bool Thread::IsRunning() const noexcept { return mRunning; }

std::string_view Thread::GetName() const noexcept { return mName; }
} // namespace Components::Thread