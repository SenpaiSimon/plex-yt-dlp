#pragma once

#include "IItem.h"
#include "ILogger.h"

namespace Items {
class Playlist final : public IItem {
public:
  Playlist(Components::Logger::ILogger& logger);
  void PreProcess() final;
  void Process() final;
  void PostProcess() final;

private:
  Components::Logger::ILogger& mLogger;
};

}; // namespace Items