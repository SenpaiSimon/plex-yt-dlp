#include <downloader/downloader.hpp>

downloader::downloader(settings setting, json conf) {
    this->setting = setting;
    this->conf = conf;
    this->customFolder = "";
}

void downloader::setNewDlUrl(string url) {
    this->setting.dlUrl = url;
}

void downloader::preStart() {
    fs::create_directories(conf["videoPath"]);
    fs::create_directories(conf["musicPath"]);
    fs::create_directories("temp");
}

void downloader::start() {
    this->preStart();

    // first setup the output folder path
    string outputPath = "";
    string tempOutFile = "";
    string tempPath = "";
    string rssArtistName = "";

    // TODO check for missing artist or album and prompt user
    if(setting.mediaType == "video") { // single video
        cout << "==" << endl;
        if(this->customFolder.empty()) {
            cout << "== Enter " << colors::cyan("Folder Name") << ": ";
            getline(cin, this->customFolder);
        }
        outputPath = conf["videoPath"];
        tempPath = string(conf["tempPath"]) + DEFAULT_VIDEO_PATH_PATTERN(this->customFolder); 
        tempOutFile =  tempPath + DEFAULT_VIDEO_FILE_NAME;
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
        tempPath = string(conf["tempPath"]) + DEFAULT_VIDEO_PLAYLIST_PATH_PATTERN; 
        tempOutFile = tempPath + DEFAULT_VIDEO_PLAYLIST_FILE_NAME(to_string(setting.idOverwrite));
    } else if (setting.mediaType == "musicPlaylist") { // whole music playlist
        outputPath = conf["musicPath"];
        tempPath = string(conf["tempPath"]) + DEFAULT_MUSIC_PLAYLIST_PATH_PATTERN; 
        tempOutFile = tempPath + DEFAULT_MUSIC_PLAYLIST_FILE_NAME;
    } else if (setting.mediaType == "rss") { // single rss feed
        cout << "==" << endl;
        cout << "== Enter " << colors::cyan("Artist Name") << ": ";
        getline(cin, rssArtistName);
        outputPath = conf["rssPath"];
        tempPath = string(conf["tempPath"]) + DEFAULT_RSS_PLAYLIST_PATH_PATTERN; 
        tempOutFile = tempPath + DEFAULT_RSS_PLAYLIST_FILE_NAME;
    }

    // post command hook
    string finalFilePath = tempOutFile;
    // first remove the ./temp dir prefix
    string toBeRemoved = string(conf["tempPath"]);
    finalFilePath = finalFilePath.erase(finalFilePath.find(toBeRemoved), toBeRemoved.length());
    // then remove the file name
    string finalFolderPath = finalFilePath.erase(finalFilePath.find_last_of("/"), finalFilePath.length() - finalFilePath.find_last_of("/"));

    #ifdef __WIN32 // TODO command for windows
    tempPath.append("//");
    string preHook = "\"xcopy /y \"\"" + tempPath + "\"\"\"\" \"\"" + outputPath + finalFolderPath + "\\\\\"\"\"\"\"";
    std::replace(preHook.begin(), preHook.end(), '/', '\\');
    preHook = std::regex_replace(preHook, std::regex("\\\\y"), "/y", std::regex_constants::format_first_only);
    string postHook = "\"rmdir /s /q \"\"" + tempPath + "\"\"\"";
    std::replace(postHook.begin(), postHook.end(), '/', '\\');
    postHook = std::regex_replace(postHook, std::regex("\\\\s"), "/s", std::regex_constants::format_first_only);
    postHook = std::regex_replace(postHook, std::regex("\\\\q"), "/q", std::regex_constants::format_first_only);
    #else
    string preHook = "\'mkdir -p \"" + outputPath + finalFolderPath + "\"\'";
    string postHook = "\'mv \"" + tempPath + "\"/* \"" + outputPath + finalFolderPath + "\"/\'";
    #endif

    // download video
    string downloadCommand = "yt-dlp \"" + setting.dlUrl + "\"";
    downloadCommand += " --exec " + preHook;
    downloadCommand += " --exec " + postHook;

    if(setting.mediaType == "video" || setting.mediaType == "videoPlaylist") { 
        downloadCommand += " --sub-format best --sub-langs de.*,ger.*,en.*";
        downloadCommand += " --embed-subs --embed-thumbnail --embed-metadata --embed-chapters --write-info-json";
        downloadCommand += " --write-thumbnail --convert-thumbnails jpg";
        downloadCommand += " -f \"bestvideo[ext=mp4]+bestaudio[ext=m4a]/mp4\" -o \"" + tempOutFile + "\"";
    } else if (setting.mediaType == "music" || setting.mediaType == "musicPlaylist") {
        downloadCommand += " --embed-thumbnail --embed-metadata";
        downloadCommand += " --parse-metadata \"playlist_autonumber:%(track_number)s\"";
        downloadCommand += " --parse-metadata \"playlist_title:%(album)s\"";
        downloadCommand += " --add-metadata --embed-chapters";
        downloadCommand += " -f \"bestaudio[ext=m4a]/bestaudio[ext=aac]/bestaudio[ext=mp3]\" -o \"" + tempOutFile + "\"";
    } else if (setting.mediaType == "rss") {
        downloadCommand += " --embed-thumbnail --embed-metadata";
        downloadCommand += " --parse-metadata \"playlist_autonumber:%(track_number)s\"";
        downloadCommand += " --parse-metadata \"playlist_title:%(album)s\"";
        downloadCommand += " --parse-metadata \"playlist_autonumber:%(artist)s\"";
        downloadCommand += " --replace-in-metadata \"artist\" \"\\d+\" \"" + rssArtistName +"\"";
        downloadCommand += " --add-metadata --embed-chapters";
        downloadCommand += " -f \"bestaudio[ext=m4a]/bestaudio[ext=aac]/bestaudio[ext=mp3]\" -o \"" + tempOutFile + "\"";
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
}

void downloader::postStart() {
    fs::remove_all("temp");
}