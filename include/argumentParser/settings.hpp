#pragma once

#include <string>
#include <stdbool.h>

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef _WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

using std::string;

class settings
{
    private:
        
    public:
        bool showHelp;
        bool showVersion;
        
        bool list;
        string linkList;

        string dlUrl;
        string mediaType;

        int indexOverwrite;
        int idOverwrite;

        bool reverse;

        string album;
        string artist;

        string cwd;

        bool jellyfin;
        string playlistPath;
        settings();
};
