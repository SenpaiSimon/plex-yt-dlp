#include <idExtractor.hpp>

idExtractor::idExtractor(settings *setting, json conf) {
    this->setting = setting;
    this->conf = conf;
}

void idExtractor::extractData() {
    // these two types dont need id extraction
    if(setting->mediaType == "music" || setting->mediaType == "video") {
        if(setting->idOverwrite == -1) {
            setting->idOverwrite = 1;
        }
        if(setting->indexOverwrite == -1) {
            setting->indexOverwrite = 1;
        }

        return;
    }

    cout << "==" << endl;
    cout << "== " << colors::boldCyan("Generating auto ID and auto Index...") << endl;
    cout << "==\t" << colors::cyan("- Scraping filespaths");

    string playlistPath = "";
    string channelPath = "";
    string playlistCount = "";
    int episodeCount = 0;
    int id = 0;

    // setup paths and names
    if(setting->mediaType == "videoPlaylist") {
        // this path is without a ending slash /
        if(setting->jellyfin) {
            playlistPath = string(conf["videoPath"]) + \
                tools::executeCommand(YT_DLP_PARSE_PATH(JELLY_VIDEO_PLAYLIST_PATH_PATTERN("%(playlist_title)s"), setting->dlUrl));
            channelPath = playlistPath;
        } else {
            playlistPath = string(conf["videoPath"]) + \
                tools::executeCommand(YT_DLP_PARSE_PATH(DEFAULT_VIDEO_PLAYLIST_PATH_PATTERN, setting->dlUrl));
            channelPath = playlistPath;
            channelPath = channelPath.erase(channelPath.find_last_of("/"));
        }
    } 

    if(setting->mediaType == "musicPlaylist") {
        // this path is without a ending slash /
        playlistPath = string(conf["musicPath"]) + \
            tools::executeCommand(YT_DLP_PARSE_PATH(DEFAULT_MUSIC_PLAYLIST_PATH_PATTERN(setting->artist), setting->dlUrl));
        // also without a ending slash /
        channelPath = playlistPath;
        channelPath = channelPath.erase(channelPath.find_last_of("/"));
    }
    
    if(setting->mediaType == "rss") { 
        // this path is without a ending slash /
        playlistPath = string(conf["rssPath"]) + \
            tools::executeCommand(YT_DLP_PARSE_PATH(DEFAULT_RSS_PLAYLIST_PATH_PATTERN, setting->dlUrl));
        playlistCount = tools::executeCommand(YT_DLP_PARSE_PATH("%(playlist_count)s", setting->dlUrl));
    }

    setting->playlistPath = playlistPath;
    cout << colors::boldGreen(" - done") << endl;

    // only find out which index to start from
    //  its a existing playlist from and existing channel
    if(fs::exists(playlistPath)) {
        cout << "==\t" << colors::cyan("- Found existing Playlist from existing Channel");

        // this can run for videosPlaylist, musicPlaylist and also RSS
        for (const auto& entry : std::filesystem::directory_iterator(playlistPath)) {
            // skip the info files
            if(entry.path().extension() == ".mp4" || \
               entry.path().extension() == ".m4a" || \
               entry.path().extension() == ".mp3" || \
               entry.path().extension() == ".aac") { 
                episodeCount++;
            }
        }

        if(setting->indexOverwrite == -1) {
            if(setting->reverse == true) {
                // if its reverse, index from the back
                setting->indexOverwrite = std::stoi(playlistCount) - episodeCount;
            } else {
                // its +1 because we want the next one to be downloaded
                setting->indexOverwrite = episodeCount + 1;
            }
        }

        // this does not need to run for jellyfin
        if(setting->mediaType == "videoPlaylist" != setting->jellyfin) {
            for (const auto& entry : std::filesystem::directory_iterator(playlistPath)) {
                // get the filename and extract the id by Season
                string fileName = entry.path().filename().string();
                // match for pattern SxEx
                string match = tools::getRegexMatches(fileName, "S([0-9]+)E([0-9]+)")[0];

                // if its empty just skip it
                if(match.empty()) {
                    continue;
                } else {
                    // strip the episode number
                    match = tools::getRegexMatches(match, "S([0-9]+)")[0];
                    // else get the biggest of all ids
                    int curId = std::stoi(match.substr(1));
                    if(curId > id) {
                        id = curId;
                    }
                }
            }

            if(setting->idOverwrite == -1) {
                setting->idOverwrite = id;
            }
        }

        // we are done here
        cout << colors::boldGreen(" - done") << endl;
        cout << "==" << endl;
        tools::printLine();
        return;
    }

    // its a new playlist from an existing channel
    if(fs::exists(channelPath)) {
        cout << "==\t" << colors::cyan("- Found new Playlist from existing Channel");

        // if its reverse we have to index from the back
        if(setting->indexOverwrite == -1) {
            if(setting->reverse == true) {
                setting->indexOverwrite = std::stoi(playlistCount);
            } else {
                setting->indexOverwrite = 1;
            }
        }

        // now generate a new id but only for video playlists
        if(setting->mediaType == "videoPlaylist") {
            for (const auto& entry : std::filesystem::directory_iterator(channelPath)) {
                if (std::filesystem::is_directory(entry.path())) {
                    id++;
                }
            }

            // its +1 because we want a new id
            if(setting->idOverwrite == -1) {
                setting->idOverwrite = id + 1;
            }
        }

        // we are done here
        cout << colors::boldGreen(" - done") << endl;
        cout << "==" << endl;
        tools::printLine();
        return;
    }

    cout << "==\t" << colors::cyan("- Found new Playlist from new Channel");
    // its a new channel and a new playlist
    if(setting->idOverwrite == -1) {
        setting->idOverwrite = 1;
    }

    // if its reverse we have to index from the back
    if(setting->indexOverwrite == -1) {
        if(setting->reverse == true) {
            setting->indexOverwrite = std::stoi(playlistCount);
        } else {
            setting->indexOverwrite = 1;
        }
    }

    cout << colors::boldGreen(" - done") << endl;
    cout << "==" << endl;
    tools::printLine();
}