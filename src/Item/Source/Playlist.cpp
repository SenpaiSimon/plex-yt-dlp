#include "Playlist.h"

using namespace Components::Logger;

namespace Items {
Playlist::Playlist(ILogger& logger) : mLogger(logger) {};

void Playlist::PreProcess() { mLogger.Info(Components::Logger::Category::Processing, LogMessage("Pre-processing playlist...")); };

void Playlist::Process() { mLogger.Info(Components::Logger::Category::Processing, LogMessage("Processing playlist...")); };

void Playlist::PostProcess() { mLogger.Info(Components::Logger::Category::Processing, LogMessage("Post-processing playlist...")); };

}; // namespace Items