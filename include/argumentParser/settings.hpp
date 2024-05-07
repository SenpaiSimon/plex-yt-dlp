#pragma once

#include <string>
#include <stdbool.h>

using std::string;

class settings
{
    private:
        
    public:
        bool showHelp;
        bool showVersion;

        string dlUrl;
        string mediaType;

        int indexOverwrite;
        int idOverwrite;

        settings();
};
