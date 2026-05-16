#include "Logger.h"
#include <chrono>
#include <memory>
#include <thread>

using Components::Logger::Category;
using Components::Logger::LogMessage;

int main(int argc, char** argv) {
  std::unique_ptr<Components::Logger::ILogger> logger = std::make_unique<Components::Logger::Logger>();

  logger->Info(Category::Init, LogMessage("Application started"));
  logger->Trace(Category::Download, LogMessage("Downloading video..."));
  logger->Warn(Category::Processing, LogMessage("Processing might take a while..."));
  logger->Error(Category::Network, LogMessage("Failed to connect to server"));

  std::this_thread::sleep_for(std::chrono::seconds(1));

  logger->Info(Category::Init, LogMessage("Application stopped"));

  return 0;
}