#include <downloader/downloader.hpp>

downloader::downloader(settings setting, json conf) {
    this->setting = setting;
    this->conf = conf;
}

void downloader::preStart() {
    fs::create_directories(conf["videoPath"]);
    fs::create_directories(conf["musicPath"]);
    fs::create_directories("temp");
}

void downloader::start() {
    this->preStart();

    this->postStart();   
}

void downloader::postStart() {
    fs::remove_all("temp");
}