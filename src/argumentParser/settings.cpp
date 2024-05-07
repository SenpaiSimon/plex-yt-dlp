#include <argumentParser/settings.hpp>

settings::settings() {
    this->showHelp = false;
    this->showVersion = false;
    this->dlUrl = "defaultUrl";
    this->mediaType = "defaultMedia";
    this->indexOverwrite = -1;
    this->idOverwrite = -1;
}