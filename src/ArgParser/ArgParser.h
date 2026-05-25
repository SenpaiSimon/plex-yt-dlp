#pragma once

#include "ILogger.h"

namespace Components {
class ArgParser {
public:
  ArgParser(int argc, char** argv, Components::Logger::ILogger& logger);

private:
  Components::Logger::ILogger& mLogger;
};
} // namespace Components