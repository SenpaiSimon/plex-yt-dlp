#include <argumentParser/settings.hpp>

settings::settings() {
    this->showHelp = false;
    this->showVersion = false;
    this->list = false;
    this->linkList = "";
    this->dlUrl = "defaultUrl";
    this->mediaType = "defaultMedia";
    this->indexOverwrite = -1;
    this->idOverwrite = -1;
    this->reverse = false;
    this->album = "";
    this->artist = "";
    this->jellyfin = false;
    this->debug = false;

    char cCurrentPath[FILENAME_MAX];
    if (GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
        this->cwd = string(cCurrentPath);
    }
}