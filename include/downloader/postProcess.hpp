#pragma once

#include <argumentParser/settings.hpp>
#include <nlohmann/json.hpp>
#include <vector>

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <algorithm> // for std::sort
#include <regex> // for std::regex
#include <tinyxml2.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using json = nlohmann::json;
namespace fs = std::filesystem;

typedef struct nfoInfo {
    string title;
    string showtitle;
    string uniqueid;
    string plot;
    string premiered;
    string director;
    int displayOrder;
} nfoInfo_t;

class postProcess {
    private:
        settings setting;
        nfoInfo_t nfoInfo;
        int curIndex = 1;

        void processEpisodes(vector<fs::path> episodeInfos, vector<fs::path> images);
        void processPlaylist(fs::path info, fs::path image);

        nfoInfo_t scrapePath(fs::path path, bool episode);
        void createNfo(fs::path path, nfoInfo_t nfo, bool episode);
        std::string reformatDate(const std::string& date);
        bool isPlaylistMeta(fs::path path);

    public:
        postProcess(settings setting);
        ~postProcess() = default;

        void convert();
};

