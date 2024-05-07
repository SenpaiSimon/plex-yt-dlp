#include <idExtractor.hpp>

idExtractor::idExtractor(settings *setting, json conf) {
    this->setting = setting;
    this->conf = conf;
}

void idExtractor::extractData() {
    string channelTitle;
    string playlistTitle;

    if(setting->mediaType == "videoPlaylist" || setting->mediaType == "musicPlaylist") {
        // retrieve channel name and title to build filepath
        string playlistId = tools::getRegexMatches(setting->dlUrl, "PL[\\w-]*")[0];

        // create api request
        string apiRequest = "https://www.googleapis.com/youtube/v3/playlists?part=snippet&id=" + playlistId \
                          + string("&key=") + string(conf["apiKey"]);

        // call and parse request
        string res = tools::getRequest(apiRequest);
        json parsedRes = json::parse(res)["items"][0]["snippet"];

        // extract data from request
        channelTitle = parsedRes["channelTitle"];
        playlistTitle = string(parsedRes["title"]);
        if(setting->mediaType == "videoPlaylist") {
            playlistTitle += "[youtube-" + playlistId + "]";
        }

        // check if playlist exists
        string channelPath = (setting->mediaType == "videoPlaylist" ? string(conf["videoPath"]) : string(conf["musicPath"]))\
                           + "/" + channelTitle;
        string playlistPath = channelPath + "/" + playlistTitle;

        if(fs::exists(playlistPath)) {
            // playlist has been downloaded at some point -- get the season and highest episode number
            vector<string> filesInDir;
            int id;
            int nextEpisode;
            vector<int> episodesDownloaded;

            // loop through files in playlist directory
            if(setting->mediaType == "videoPlaylist") {
                for (const auto& entry : std::filesystem::directory_iterator(playlistPath)) {
                    string fileName = entry.path().filename();
                    filesInDir.push_back(fileName);

                    id = std::stoi(string(tools::getRegexMatches(fileName, "S([0-9]+)")[0]).substr(1));
                    int curEpisode = std::stoi(string(tools::getRegexMatches(fileName, "E([0-9]+)")[0]).substr(1));
                    episodesDownloaded.push_back(curEpisode);
                }
                // get the highest episode number and add 1 to it
                nextEpisode = *max_element(episodesDownloaded.begin(), episodesDownloaded.end()) + 1;

                // only playlists of type video have an id
                if(setting->idOverwrite == -1) {
                    setting->idOverwrite = id;
                }
            } else { // its type musicPlaylist
                // count the number of files already there
                for (const auto& entry : std::filesystem::directory_iterator(playlistPath)) {
                    nextEpisode++;
                }
                // increment it by one more
                nextEpisode++;
            }

            // commit it to the settings, but only if no forced overwrite is set
            if(setting->indexOverwrite == -1) {
                setting->indexOverwrite = nextEpisode;
            }
        } else {
            // its a new playlist
            int channelPlaylists = 0;
            if(fs::exists(channelPath) && setting->mediaType == "videoPlaylist") {
                for (const auto& entry : std::filesystem::directory_iterator(channelPath)) {
                    if (std::filesystem::is_directory(entry.path())) {
                        channelPlaylists++;
                    }
                }
                if(setting->idOverwrite == -1) {
                    setting->idOverwrite = channelPlaylists + 1;
                }
            } else {
                // its the first one from this channel so just use one as an id
                if(setting->idOverwrite == -1) {
                    setting->idOverwrite = 1;
                }
            }
            // since its a new playlist and a new channel also download from the first video
            if(setting->indexOverwrite == -1) {
                setting->indexOverwrite = 1;
            }
        }
    } else { // just set it to the beginning for everything else if no overwrite is set
        if(setting->idOverwrite == -1) {
            setting->idOverwrite = 1;
        }
        if(setting->indexOverwrite == -1) {
            setting->indexOverwrite = 1;
        }
    }
}