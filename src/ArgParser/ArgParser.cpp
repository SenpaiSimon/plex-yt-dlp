#include "ArgParser.h"

using namespace Components::Logger;

namespace Components {
ArgParser::ArgParser(int argc, char** argv, ILogger& logger) : mLogger(logger) {}
} // namespace Components