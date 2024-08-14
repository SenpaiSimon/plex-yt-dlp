#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <misc/tools.hpp>

#define DEFAULT_MUSIC_PATH_PATTERN(folder) "/%(uploader)s/" + folder
#define DEFAULT_MUSIC_FILE_NAME "/%(title)s [%(id)s].%(ext)s"

#define DEFAULT_VIDEO_PATH_PATTERN(folder) "/%(uploader)s/" + folder
#define JELLY_VIDEO_PATH_PATTERN(folder)   "/%(uploader)s [%(uploader_id)s]/" + folder
#define DEFAULT_VIDEO_FILE_NAME "/%(title)s [%(id)s].%(ext)s"

#define DEFAULT_VIDEO_PLAYLIST_PATH_PATTERN "/%(uploader)s/%(playlist_title)s [youtube-%(playlist_id)s]"
#define JELLY_VIDEO_PLAYLIST_PATH_PATTERN   "/%(uploader)s [%(uploader_id)s]/%(playlist_title)s"
#define DEFAULT_VIDEO_PLAYLIST_FILE_NAME(id) "/%(title)s - S" + id + "E%(playlist_index)s [%(id)s].%(ext)s"

#define DEFAULT_MUSIC_PLAYLIST_PATH_PATTERN "/%(uploader)s/%(playlist_title)s"
#define DEFAULT_MUSIC_PLAYLIST_FILE_NAME "/%(title)s.%(ext)s"

#define DEFAULT_RSS_PLAYLIST_PATH_PATTERN "/%(playlist_title)s"
#define DEFAULT_RSS_PLAYLIST_FILE_NAME "/%(title)s.%(ext)s"

#define YT_DLP_PARSE_PATH(pattern, url) string("yt-dlp -s --no-warnings -I 1 --print \"") + pattern + "\" " + url


using json = nlohmann::json;
using std::string;

class config
{
    private:
        config();
        
    public:
        static json parseConfig(string cwd);

};