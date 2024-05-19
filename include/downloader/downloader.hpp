#pragma once

#include <nlohmann/json.hpp>
#include <misc/tools.hpp>
#include <config.hpp>
#include <argumentParser/settings.hpp>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

#include <string>
#include <filesystem>
#include <iostream>

using json = nlohmann::json;
namespace fs = std::filesystem;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::stoi;
using std::to_string;

class downloader {
    private:
        settings setting;
        json conf;
        string customFolder;

        void preStart();
        void postStart();
    public:
        void setNewDlUrl(string url);
        downloader(settings setting, json conf);
        void start();
};
