#include "Single.h"

using namespace Components::Logger;

namespace Items {
Single::Single(Components::Logger::ILogger& logger) : mLogger(logger) {};

void Single::PreProcess() { mLogger.Info(Components::Logger::Category::Processing, LogMessage("Pre-processing single...")); };

void Single::Process() { mLogger.Info(Components::Logger::Category::Processing, LogMessage("Processing single...")); };

void Single::PostProcess() { mLogger.Info(Components::Logger::Category::Processing, LogMessage("Post-processing single...")); };

}; // namespace Items