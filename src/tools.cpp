#include <tools.hpp>

void tools::printLine() {
  cout << "=====================================================================================" << endl;
}

bool tools::checkIfIsExecutable(const string& name) {
    if (access(name.c_str(), X_OK) == -1) {
        return false;
    } else {
        return true;
    }
}

string tools::getExecutablePath(const string& name) {
    #ifdef __WIN32
    string command = "where " + name;
    #else
    string command = "which " + name;
    #endif
    string result = tools::executeCommand(command);
    return result;
}

string tools::executeCommand(const string& command) {
    FILE *fp = popen(command.c_str(), "r");
    if(fp == NULL) {
        return "";
    }
    char buffer[128];
    string result = "";
    while(fgets(buffer, sizeof(buffer), fp) != NULL) {
        result += buffer;
    }
    pclose(fp);
    return result;
}

void tools::checkRequirements() {
    tools::printLine();

    cout << "==" << endl;
    cout << "==" << colors::boldCyan(" Cheching requirements...") << endl;

    cout << "==" << colors::cyan("\t- Searching yt-dlp");
    string yt_dlp_path = tools::getExecutablePath("yt-dlp");
    if(yt_dlp_path == "") {
        cout << colors::boldRed(" - missing! ") << "Install from here: https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp" << endl;
        exit(1);
    } else {
        cout << colors::boldGreen(" - found!") << endl;
    }

    cout << "==" << colors::cyan("\t- Searching ffmpeg");
    string ffmpeg_path = tools::getExecutablePath("ffmpeg");
    if(ffmpeg_path == "") {
        cout << colors::boldRed(" - missing! ");
        #ifdef __WIN32
        cout << "Install with command: winget install -e --id Gyan.FFmpeg" << endl;
        #else
        cout << "Install with command: sudo apt install ffmpeg" << endl;
        #endif
        exit(1);
    } else {
        cout << colors::boldGreen(" - found!") << endl;
    }

    cout << "==" << colors::cyan("\t- Searching Curl");
    string curl_path = tools::getExecutablePath("curl");
    if(curl_path == "") {
        cout << colors::boldRed(" - missing!") << endl;
        exit(1);
    } else {
        cout << colors::boldGreen(" - found!") << endl;
    }

    cout << "==" << endl;
    tools::printLine();
}

void tools::printHelp() {
    cout << "help TODO" << endl;
}

void tools::printVersion() {
    cout << "Version is: " << CUR_VERSION << endl;
}