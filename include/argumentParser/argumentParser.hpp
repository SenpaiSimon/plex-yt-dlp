#pragma once

#include <string>
#include <iostream>
#include <stdbool.h>
#include <regex>

#include <argumentParser/settings.hpp>
#include <colors.hpp>

using std::string;
using std::cout;
using std::endl;

class argumentParser
{
    private:
        /* data */
        int argCount;
        char** argValues;

    public:
        settings setting;
        argumentParser(int argc, char** argv);
        void parse();
        void checkArguments();
};

