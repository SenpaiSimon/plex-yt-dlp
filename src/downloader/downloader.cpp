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
    string outFile = "";
    string customFolder = "";
    if(setting.mediaType == "video") { // single video
        cout << "==" << endl;
        cout << "== Enter " << colors::cyan("Folder Name") << ": ";
        getline(cin, customFolder);
        outputPath = conf["videoPath"];
        outFile = string(conf["tempPath"]) + "/%(uploader)s/" + customFolder + "/%(title)s [%(id)s].%(ext)s";
    } else if (setting.mediaType == "music") { // single music file
        cout << "==" << endl;
        cout << "== Enter " << colors::cyan("Folder Name") << ": ";
        getline(cin, customFolder);
        outputPath = conf["musicPath"];
        outFile = string(conf["tempPath"]) + "/%(uploader)s/" + customFolder + "/%(title)s [%(id)s].%(ext)s";
    } else if (setting.mediaType == "videoPlaylist") { // whole video playlist
        outputPath = conf["videoPath"];
        outFile = string(conf["tempPath"]) + "/%(uploader)s/%(playlist_title)s [youtube-%(playlist_id)s]/%(title)s - S" + to_string(setting.idOverwrite) + "E%(playlist_index)s [%(id)s].%(ext)s";
    } else if (setting.mediaType == "musicPlaylist") { // whole music playlist
        outputPath = conf["musicPath"];
        outFile = string(conf["tempPath"]) + "/%(uploader)s/%(playlist_title)s/%(title)s.%(ext)s";
    } else if (setting.mediaType == "rss") { // single rss feed
        outputPath = conf["rssPath"];
        outFile = string(conf["tempPath"]) + "/%(uploader)s/%(playlist_title)s/%(title)s.%(ext)s";
    }

    // post command hook
    #ifdef __WIN32
    string postHook = "tempHook";
    #else
    string postHook = "tempHook";
    #endif

    // download video
    string downloadCommand = "yt-dlp " + setting.dlUrl;
    if(setting.mediaType == "video" || setting.mediaType == "videoPlaylist") { 
        downloadCommand += " --sub-format best --sub-langs de.*,ger.*,en.*";
        downloadCommand += " --embed-subs --embed-thumbnail --embed-metadata --embed-chapters --write-info-json";
        downloadCommand += " -f 'bestvideo[ext=mp4]+bestaudio[ext=m4a]/mp4' -o \"";
        downloadCommand += outFile + "\" -I " + to_string(setting.indexOverwrite) + "::1";
        downloadCommand += " --exec " + postHook;
    } else if (setting.mediaType == "music" || setting.mediaType == "musicPlaylist") {
        downloadCommand += " --embed-thumbnail --embed-metadata";
        downloadCommand += " --parse-metadata \"playlist_autonumber:%(track_number)s\"";
        downloadCommand += " --add-metadata --embed-chapters";
        downloadCommand += " -f 'bestaudio[ext=m4a]/bestaudio[ext=aac]/bestaudio[ext=mp3]' -o \"";
        downloadCommand += outFile + "\" -I " + to_string(setting.indexOverwrite) + "::1";
        downloadCommand += " --exec " + postHook;
    } else if (setting.mediaType == "rss") {
        downloadCommand += " --embed-thumbnail --embed-metadata";
        downloadCommand += " --parse-metadata \"playlist_autonumber:%(track_number)s\"";
        downloadCommand += " --add-metadata --embed-chapters --playlist-reverse";
        downloadCommand += " -f 'bestaudio[ext=m4a]/bestaudio[ext=aac]/bestaudio[ext=mp3]' -o \"";
        downloadCommand += outFile + "\" -I " + to_string(setting.indexOverwrite) + "::1";
        downloadCommand += " --exec " + postHook;
    }

    // issue the command
    cout << "=="     << endl;
    cout << "== "    << colors::boldCyan("Starting Download...") << endl;
    cout << "==\t- " << colors::cyan("Media Type: ") << colors::boldGreen(setting.mediaType) << endl;
    cout << "==\t- " << colors::cyan("Download URL: ") << colors::boldGreen(setting.dlUrl) << endl;
    cout << "==\t- " << colors::cyan("Starting Index: ") << colors::boldGreen(to_string(setting.indexOverwrite)) << endl;
    if(setting.mediaType == "videoPlaylist") {
        cout << "==\t- " << colors::cyan("Starting ID: ") << colors::boldGreen(to_string(setting.idOverwrite)) << endl;
    }
    cout << "==\t- " << colors::cyan("Output Path: ") << colors::boldGreen(outputPath) << endl;

    // post
    cout << "==" << endl;
    tools::printLine();
    this->postStart();   
}

void downloader::postStart() {
    fs::remove_all("temp");
}