#include "Logger.h"
#include "Playlist.h"
#include <chrono>
#include <memory>
#include <thread>

using Components::Logger::Category;
using Components::Logger::LogMessage;

int main(int argc, char** argv) {
  std::unique_ptr<Components::Logger::ILogger> logger = std::make_unique<Components::Logger::Logger>();

  Items::Playlist playlist(*logger);
  playlist.PreProcess();
  playlist.Process();
  playlist.PostProcess();

  std::this_thread::sleep_for(std::chrono::seconds(1)); // Allow logger thread to flush messages before exiting.

  return 0;
}