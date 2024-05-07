#pragma once

#include <string>
#include <iostream>
#include <stdbool.h>
#include <regex>

#include <argumentParser/settings.hpp>
#include <misc/colors.hpp>
#include <misc/tools.hpp>

using std::string;
using std::cout;
using std::endl;

class argumentParser
{
    private:
        /* data */
        int argCount;
        char** argValues;
        settings setting;

    public:
        argumentParser(int argc, char** argv);
        settings parse();
        void checkArguments();
};

