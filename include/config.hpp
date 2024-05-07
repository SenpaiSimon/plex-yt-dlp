#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <misc/tools.hpp>

using json = nlohmann::json;
using std::string;

class config
{
    private:
        config();
        
    public:
        static json parseConfig(string cwd);

};