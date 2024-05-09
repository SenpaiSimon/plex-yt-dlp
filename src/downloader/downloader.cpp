#include <downloader/downloader.hpp>

downloader::downloader(settings setting, json conf) {
    this->setting = setting;
    this->conf = conf;
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
    string customFolder = "";
    if(setting.mediaType == "video") { // single video
        cout << "==" << endl;
        cout << "== Enter " << colors::cyan("Folder Name") << ": ";
        getline(cin, customFolder);
        outputPath = conf["videoPath"];
        tempPath = string(conf["tempPath"]) + "/%(uploader)s/" + customFolder; 
        tempOutFile =  tempPath + "/%(title)s [%(id)s].%(ext)s";
    } else if (setting.mediaType == "music") { // single music file
        cout << "==" << endl;
        cout << "== Enter " << colors::cyan("Folder Name") << ": ";
        getline(cin, customFolder);
        outputPath = conf["musicPath"];
        tempPath = string(conf["tempPath"]) + "/%(uploader)s/" + customFolder;
        tempOutFile =  tempPath + "/%(title)s [%(id)s].%(ext)s";
    } else if (setting.mediaType == "videoPlaylist") { // whole video playlist
        outputPath = conf["videoPath"];
        tempPath = string(conf["tempPath"]) + "/%(uploader)s/%(playlist_title)s [youtube-%(playlist_id)s]"; 
        tempOutFile = tempPath + "/%(title)s - S" + to_string(setting.idOverwrite) + "E%(playlist_index)s [%(id)s].%(ext)s";
    } else if (setting.mediaType == "musicPlaylist") { // whole music playlist
        outputPath = conf["musicPath"];
        tempPath = string(conf["tempPath"]) + "/%(uploader)s/%(playlist_title)s"; 
        tempOutFile = tempPath + "/%(title)s.%(ext)s";
    } else if (setting.mediaType == "rss") { // single rss feed
        outputPath = conf["rssPath"];
        tempPath = string(conf["tempPath"]) + "/%(uploader)s/%(playlist_title)s"; 
        tempOutFile = tempPath + "/%(title)s.%(ext)s";
    }

    // post command hook
    string finalFilePath = tempOutFile;
    // first remove the ./temp dir prefix
    string toBeRemoved = string(conf["tempPath"]);
    finalFilePath = finalFilePath.erase(finalFilePath.find(toBeRemoved), toBeRemoved.length());
    // then remove the file name
    string finalFolderPath = finalFilePath.erase(finalFilePath.find_last_of("/"), finalFilePath.length() - finalFilePath.find_last_of("/"));

    #ifdef __WIN32 // TODO command for windows
    string preHook = "mkdir -p \"" + outputPath + finalFolderPath + "\"";
    string postHook = "mv \"" + tempPath + "\"/* \"" + outputPath + finalFolderPath + "\"";
    #else
    string preHook = "mkdir -p \"" + outputPath + finalFolderPath + "\"";
    string cdHook = "cd \"" + tempPath + "\"";
    string postHook = "mv \"" + tempPath + "\"/* \"" + outputPath + finalFolderPath + "\"/";
    #endif

    // download video
    string downloadCommand = "yt-dlp " + setting.dlUrl;
    if(setting.mediaType == "video" || setting.mediaType == "videoPlaylist") { 
        downloadCommand += " --sub-format best --sub-langs de.*,ger.*,en.*";
        downloadCommand += " --embed-subs --embed-thumbnail --embed-metadata --embed-chapters --write-info-json";
        downloadCommand += " -f 'bestvideo[ext=mp4]+bestaudio[ext=m4a]/mp4' -o \"";
        downloadCommand += tempOutFile + "\" -I " + to_string(setting.indexOverwrite) + "::1";
        downloadCommand += " --exec \'" + preHook + "\'";
        downloadCommand += " --exec \'" + postHook + "\'";
    } else if (setting.mediaType == "music" || setting.mediaType == "musicPlaylist") {
        downloadCommand += " --embed-thumbnail --embed-metadata";
        downloadCommand += " --parse-metadata \"playlist_autonumber:%(track_number)s\"";
        downloadCommand += " --add-metadata --embed-chapters";
        downloadCommand += " -f 'bestaudio[ext=m4a]/bestaudio[ext=aac]/bestaudio[ext=mp3]' -o \"";
        downloadCommand += tempOutFile + "\" -I " + to_string(setting.indexOverwrite) + "::1";
        downloadCommand += " --exec \'" + preHook + "\'";
        downloadCommand += " --exec \'" + postHook + "\'";
    } else if (setting.mediaType == "rss") {
        downloadCommand += " --embed-thumbnail --embed-metadata";
        downloadCommand += " --parse-metadata \"playlist_autonumber:%(track_number)s\"";
        downloadCommand += " --add-metadata --embed-chapters --playlist-reverse";
        downloadCommand += " -f 'bestaudio[ext=m4a]/bestaudio[ext=aac]/bestaudio[ext=mp3]' -o \"";
        downloadCommand += tempOutFile + "\" -I " + to_string(setting.indexOverwrite) + "::1";
        downloadCommand += " --exec \'" + preHook + "\'";
        downloadCommand += " --exec \'" + postHook + "\'";
    }

    // print info before the command call
    cout << "=="     << endl;
    cout << "== "    << colors::boldCyan("Starting Download...") << endl;
    cout << "==\t- " << colors::cyan("Media Type: ") << colors::boldGreen(setting.mediaType) << endl;
    cout << "==\t- " << colors::cyan("Download URL: ") << colors::boldGreen(setting.dlUrl) << endl;
    cout << "==\t- " << colors::cyan("Starting Index: ") << colors::boldGreen(to_string(setting.indexOverwrite)) << endl;
    if(setting.mediaType == "videoPlaylist") {
        cout << "==\t- " << colors::cyan("Starting ID: ") << colors::boldGreen(to_string(setting.idOverwrite)) << endl;
    }
    cout << "==\t- " << colors::cyan("Output Path: ") << colors::boldGreen(outputPath) << endl;
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