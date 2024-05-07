#pragma once

#include <nlohmann/json.hpp>
#include <misc/tools.hpp>
#include <config.hpp>
#include <argumentParser/settings.hpp>

#include <string>
#include <filesystem>
#include <iostream>

using json = nlohmann::json;
namespace fs = std::filesystem;
using std::string;
using std::cout;
using std::endl;

class downloader {
    private:
        settings setting;
        json conf;

        void preStart();
        void postStart();
    public:
        downloader(settings setting, json conf);
        void start();
};
