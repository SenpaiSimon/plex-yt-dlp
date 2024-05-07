#pragma once

#include <argumentParser/settings.hpp>
#include <config.hpp>
#include <misc/tools.hpp>
#include <string>
#include <regex>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

using std::vector;
using std::string;
using std::smatch;

class idExtractor {
    private:
        settings *setting;
        json conf;

    public:
        idExtractor(settings *setting, json conf);
        void extractData();
};
