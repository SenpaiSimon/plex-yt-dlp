#include <downloader/downloader.hpp>

downloader::downloader(settings setting, json conf) {
    this->setting = setting;
    this->conf = conf;
    this->customFolder = "";
    this->downloadedItems = 0;
}

void downloader::setNewDlUrl(string url) {
    this->setting.dlUrl = url;
}

void downloader::preStart() {
    fs::create_directories(conf["videoPath"]);
    fs::create_directories(conf["musicPath"]);
    fs::create_directories(conf["rssPath"]);
    fs::create_directories("temp");
}

int downloader::getDownloadedCount() {
    return this->downloadedItems;
}

void downloader::start() {
    this->preStart();

    // first setup the output folder path
    string outputPath = "";
    string tempOutFile = "";
    string tempPath = "";

    // TODO check for missing artist or album and prompt user
    if(setting.mediaType == "video") { // single video
        cout << "==" << endl;
        if(this->customFolder.empty()) {
            cout << "== Enter " << colors::cyan("Folder Name") << ": ";
            getline(cin, this->customFolder);
        }
        outputPath = conf["videoPath"];
        if(setting.jellyfin) {
            tempPath = string(conf["tempPath"]) + JELLY_VIDEO_PATH_PATTERN(this->customFolder); 
        } else {
            tempPath = string(conf["tempPath"]) + DEFAULT_VIDEO_PATH_PATTERN(this->customFolder); 
        }
        tempOutFile =  tempPath + DEFAULT_VIDEO_FILE_NAME(this->getDownloadedCount() + 1);
    } else if (setting.mediaType == "music") { // single music file
        cout << "==" << endl;
        if(this->customFolder.empty()) {
            cout << "== Enter " << colors::cyan("Folder Name") << ": ";
            getline(cin, this->customFolder);
        } 
        outputPath = conf["musicPath"];
        tempPath = string(conf["tempPath"]) + DEFAULT_MUSIC_PATH_PATTERN(this->customFolder);
        tempOutFile =  tempPath + DEFAULT_MUSIC_FILE_NAME;
    } else if (setting.mediaType == "videoPlaylist") { // whole video playlist
        outputPath = conf["videoPath"];
        if(setting.jellyfin) {
            fs::path jellyPath = setting.playlistPath;
            tempPath = string(conf["tempPath"]) + "/" +  JELLY_VIDEO_PLAYLIST_PATH_PATTERN(jellyPath.filename().string()); 
            tempOutFile = tempPath + JELLY_VIDEO_PLAYLIST_FILE_NAME;
        } else {
            tempPath = string(conf["tempPath"]) + DEFAULT_VIDEO_PLAYLIST_PATH_PATTERN; 
            tempOutFile = tempPath + DEFAULT_VIDEO_PLAYLIST_FILE_NAME(to_string(setting.idOverwrite));
        }
    } else if (setting.mediaType == "musicPlaylist") { // whole music playlist
        if(setting.artist.empty()) {
            cout << "==" << endl;
            cout << "== Enter " << colors::cyan("Artist Name") << ": ";
            getline(cin, setting.artist);
        }

        outputPath = conf["musicPath"];
        tempPath = string(conf["tempPath"]) + DEFAULT_MUSIC_PLAYLIST_PATH_PATTERN(setting.artist); 
        tempOutFile = tempPath + DEFAULT_MUSIC_PLAYLIST_FILE_NAME;
    } else if (setting.mediaType == "rss") { // single rss feed
        if(setting.artist.empty()) {
            cout << "==" << endl;
            cout << "== Enter " << colors::cyan("Artist Name") << ": ";
            getline(cin, setting.artist);
        }

        outputPath = conf["rssPath"];
        tempPath = string(conf["tempPath"]) + DEFAULT_RSS_PLAYLIST_PATH_PATTERN; 
        tempOutFile = tempPath + DEFAULT_RSS_PLAYLIST_FILE_NAME;
    }

    // post command move hooks
    string tempFolder = string(conf["tempPath"]);
    string destFolder = outputPath; 

    #ifdef _WIN32 
    // convert needed paths to windows backslash bs
    std::replace(tempFolder.begin(), tempFolder.end(), '/', '\\');
    std::replace(destFolder.begin(), destFolder.end(), '/', '\\');

    // workaground with echo at the end because yt-dlp keeps appending stupid shit at the end
    string preHook = "\"xcopy /s /y \"\""+ tempFolder + "\"\"\"\" \"\"" + destFolder + "\"\"\"\" && echo \"";
    string postHook = "\"rmdir /s /q \"\"" + tempFolder + "\"\"\"\" && echo \"";
    #else
    // workaground with echo at the end because yt-dlp keeps appending stupid shit at the end
    string postHook = "\'rsync --remove-source-files -a \"" + tempFolder + "/\" \"" + destFolder + "\" && echo \'";
    #endif

    // download video
    string downloadCommand = "yt-dlp \"" + setting.dlUrl + "\"";

    #ifdef _WIN32
    // only windows needs a pre hook
    downloadCommand += " --exec " + preHook;
    #endif

    downloadCommand += " --exec " + postHook;
    downloadCommand += " --restrict-filenames";

    if(setting.mediaType == "video" || setting.mediaType == "videoPlaylist") { 
        downloadCommand += " --sub-format best --sub-langs de.*,ger.*,en.*";
        downloadCommand += " --embed-subs --embed-thumbnail --embed-metadata --embed-chapters --write-info-json";
        downloadCommand += " --write-thumbnail --convert-thumbnails jpg";
        downloadCommand += " --replace-in-metadata \"title\" \"\\[.*?\\]\" \"""\"";
        downloadCommand += " -f \"bestvideo[ext=mp4]+bestaudio[ext=m4a]/mp4\" -o \"" + tempOutFile + "\"";
    } else if (setting.mediaType == "music" || setting.mediaType == "musicPlaylist") {
        downloadCommand += " --embed-thumbnail --embed-metadata";
        downloadCommand += " --parse-metadata \"playlist_autonumber:%(track_number)s\"";
        downloadCommand += " --parse-metadata \"playlist_title:%(album)s\"";
        downloadCommand += " --parse-metadata \"playlist_autonumber:%(artist)s\""; // hacky workaround to get it to replace stuff
        downloadCommand += " --replace-in-metadata \"artist\" \"\\d+\" \"" + setting.artist +"\"";
        downloadCommand += " --add-metadata --embed-chapters";
        downloadCommand += " -f bestaudio -o \"" + tempOutFile + "\"";
    } else if (setting.mediaType == "rss") {
        downloadCommand += " --embed-thumbnail --embed-metadata";
        downloadCommand += " --parse-metadata \"playlist_autonumber:%(track_number)s\"";
        downloadCommand += " --parse-metadata \"playlist_title:%(album)s\"";
        downloadCommand += " --parse-metadata \"playlist_autonumber:%(artist)s\""; // hacky workaround to get it to replace stuff
        downloadCommand += " --replace-in-metadata \"artist\" \"\\d+\" \"" + setting.artist +"\"";
        downloadCommand += " --add-metadata --embed-chapters";
        downloadCommand += " -f \"bestaudio[ext=m4a]/bestaudio[ext=aac]/bestaudio[ext=mp3]\" -o \"" + tempOutFile + "\"";
    }

    // inject the number in the queue into the video for ordering
    if(setting.mediaType == "video") {
        downloadCommand += " --parse-metadata \"like_count:%(episode)s\""; // hacky workaround to get it to replace stuff
        downloadCommand += " --replace-in-metadata \"episode\" \"\\d+\" \"" + to_string(this->getDownloadedCount() + 1) +"\"";
    }

    if(this->setting.reverse == true) {
        downloadCommand += " --playlist-reverse";
        // reversed indexing
        downloadCommand += " -I 1:" + to_string(setting.indexOverwrite) + ":1";
    } else {
        // normal indexing
        downloadCommand += " -I " + to_string(setting.indexOverwrite) + "::1";
    }

    // print info before the command call
    cout << "=="     << endl;
    cout << "== "    << colors::boldCyan("Starting Download...") << endl;
    cout << "==\t" << colors::cyan("- Media Type: ") << colors::boldGreen(setting.mediaType) << endl;
    cout << "==\t" << colors::cyan("- Download URL: ") << colors::boldGreen(setting.dlUrl) << endl;
    cout << "==\t" << colors::cyan("- Starting Index: ") << colors::boldGreen(to_string(setting.indexOverwrite)) << endl;
    if(setting.mediaType == "videoPlaylist") {
        cout << "==\t" << colors::cyan("- Starting ID: ") << colors::boldGreen(to_string(setting.idOverwrite)) << endl;
    }
    cout << "==\t" << colors::cyan("- Output Path: ") << colors::boldGreen(outputPath) << endl;
    cout << "==" << endl;
    tools::printLine();

    // actual command call
    cout << "==" << endl;
    // Open pipe to file and call the command
    downloadCommand.append(" 2>&1"); // redirect output
    FILE* pipe = popen(downloadCommand.c_str(), "r");
    if (!pipe) {
        std::cerr << "cmd failed!" << endl;
    }

    char buffer[30];
    int segmentsDone = 0;
    string line;
    bool print = false;

    char buf;
    while (!feof(pipe) && !ferror(pipe)) {
        while(1) {
            if(feof(pipe) || ferror(pipe)) { // also stop on EOF or ERROR
                break;
            }
            buf = fgetc(pipe);
            line += buf;
            if(buf == '\n' || buf == '\r') { // stop the line on newline or carriage return
                break;
            }
        }

        // cout << line;

        // act on certain strings in the output of the command
        if(line.find("[download]") != std::string::npos) {
            if(line.find("\% of") != std::string::npos) {
                cout << "== " << colors::cyan(line);
            } else {
                if(line.find("Downloading item") != std::string::npos) {
                    cout << "==" << endl;
                    cout << "==" << endl;
                    cout << "== " << colors::boldGreen(line);
                } else {
                    cout << "== " << line;
                }
            }
        }

        if(line.find("[Exec]") != std::string::npos) {
            cout << "== " << line;
        }

        line.clear();
    }
    pclose(pipe);

    // post
    cout << "==" << endl;
    tools::printLine();
    this->postStart();   
    this->downloadedItems++;
}

void downloader::postStart() {
    fs::remove_all("temp");
}

string downloader::getCustomFolder() {
    return this->customFolder;
}