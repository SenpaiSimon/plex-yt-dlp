#include "Logger.h"
#include "Playlist.h"
#include "Single.h"
#include <chrono>
#include <memory>
#include <thread>

using Components::Logger::Category;
using Components::Logger::LogMessage;

int main(int argc, char** argv) {
  std::unique_ptr<Components::Logger::ILogger> logger = std::make_unique<Components::Logger::Logger>();
  std::queue<std::unique_ptr<Items::IItem>> itemQueue;

  itemQueue.push(std::make_unique<Items::Single>(*logger));
  itemQueue.push(std::make_unique<Items::Playlist>(*logger));

  while (!itemQueue.empty()) {
    auto item = std::move(itemQueue.front());
    itemQueue.pop();

    item->PreProcess();
    item->Process();
    item->PostProcess();
  }

  return 0;
}