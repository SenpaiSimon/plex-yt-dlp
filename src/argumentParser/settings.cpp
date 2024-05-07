#include <argumentParser/settings.hpp>

settings::settings() {
    this->showHelp = false;
    this->showVersion = false;
    this->dlUrl = "defaultUrl";
    this->mediaType = "defaultMedia";
    this->indexOverwrite = -1;
    this->idOverwrite = -1;

    char cCurrentPath[FILENAME_MAX];
    if (GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
        this->cwd = string(cCurrentPath);
    }
}