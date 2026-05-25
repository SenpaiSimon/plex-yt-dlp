#pragma once

#pragma once

#include "IItem.h"
#include "ILogger.h"

namespace Items {
class Single final : public IItem {
public:
  Single(Components::Logger::ILogger& logger);
  void PreProcess() final;
  void Process() final;
  void PostProcess() final;

private:
  Components::Logger::ILogger& mLogger;
};

}; // namespace Items