#include <misc/tools.hpp>

void tools::printLine() {
    cout << "==========================================================================================================" << endl;
}

bool tools::fileExists(string filePath) {
    return fs::exists(filePath);
}

string escapePath(const string& path) {
    #ifdef _WIN32
    const char escapeChar = '^';
    #else
    const char escapeChar = '\\';
    #endif
    // Define a set of characters that need escaping
    unordered_set<char> escapeSet = {' ', '\t', '\n', '\r', '\\'};

    string escapedPath;
    for (char c : path) {
    if (escapeSet.count(c) > 0) {
        escapedPath += escapeChar;
    }
    escapedPath += c;
    }
    return escapedPath;
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
    if(!result.empty()) {
        result.erase(result.find("\n"), 1);
    }
    pclose(fp);
    return result;
}

tools::binaryStatus_t tools::checkBinaryStatus(string name) {
    string commandRes = "";

    // check if there and executable
    #ifdef __WIN32
    string command = "where " + name;
    #else
    string command = "which " + name;
    #endif
    commandRes = tools::executeCommand(command);

    if(!commandRes.empty()) {
        return EXECUTABLE;
    } else {
        // might be there but not executable, but only on linux lol
        #ifdef __WIN32
        return MISSING;
        #else
        command = "whereis " + name;
        commandRes = tools::executeCommand(command);

        // check the output
        string toBeRemoved = name + ":";
        commandRes = commandRes.erase(commandRes.find(toBeRemoved), toBeRemoved.length());

        if(commandRes.empty()) {
            return MISSING;
        } else {
            return FOUND;
        }
        #endif
    }
}

string tools::getRequest(string req) {
    // Create a stringstream object
    ostringstream str;
    // Create a curl_ios object, passing the stream object.
    curl_ios<ostringstream> writer(str);

    curl_easy curlTool(writer);

    curlTool.add<CURLOPT_URL>(req.c_str());
    curlTool.add<CURLOPT_FOLLOWLOCATION>(1L);
    curlTool.add<CURLOPT_SSL_OPTIONS>(CURLSSLOPT_NATIVE_CA); // todo linux maybe nicht?

    try {
        curlTool.perform();
    } catch (curl_easy_exception &error) {
        // If you want to print the last error.
        std::cerr<<error.what()<<std::endl;

        // If you want to print the entire error stack you can do
        error.print_traceback();
    }

    return str.str();
}

void tools::checkRequirements() {
    tools::printLine();
    tools::binaryStatus lastStatus;

    bool missing = false;

    cout << "==" << endl;
    cout << "==" << colors::boldCyan(" Cheching requirements...") << endl;

    cout << "==" << colors::cyan("\t- Searching yt-dlp");
    lastStatus = tools::checkBinaryStatus("yt-dlp");

    if(lastStatus != EXECUTABLE) {
        missing = true;
        if(lastStatus == MISSING) {
            cout << colors::boldRed(" - missing!") << endl;
            cout << "==\t  " << colors::red("-> Install from here: ") <<  "https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp" << endl;
        } else if(lastStatus == FOUND) {
            cout << colors::boldRed(" - found, but not executable!") << endl;
            cout << "==\t  " << colors::red("-> Make it executable: ") <<  "chmod +x /path/to/your/bin/yt-dlp" << endl;
        }
    } else {
        cout << colors::boldGreen(" - found!") << endl;
    }


    cout << "==" << colors::cyan("\t- Searching ffmpeg");
    lastStatus = tools::checkBinaryStatus("ffmpeg");

    if(lastStatus != EXECUTABLE) {
        missing = true;
        cout << colors::boldRed(" - missing!") << endl;
        #ifdef __WIN32
        cout << "==\t  " << colors::red("-> Install with command: ") << "winget install -e --id Gyan.FFmpeg" << endl;
        #else
        cout << "==\t  " << colors::red("-> Install with command: ") << "sudo apt install ffmpeg" << endl;
        #endif
    } else {
        cout << colors::boldGreen(" - found!") << endl;
    }

    #ifndef __WIN32
    cout << "==" << colors::cyan("\t- Searching rsync");
    lastStatus = tools::checkBinaryStatus("rsync");

    if(lastStatus != EXECUTABLE) {
        missing = true;
        cout << colors::boldRed(" - missing!") << endl;
        cout << "==\t  " << colors::red("-> Install with command: ") << "sudo apt install rsync" << endl;
    } else {
        cout << colors::boldGreen(" - found!") << endl;
    }
    #endif


    cout << "==" << colors::cyan("\t- Searching Curl");
    lastStatus = tools::checkBinaryStatus("curl");
    if(lastStatus != EXECUTABLE) {
        missing = true;
        cout << colors::boldRed(" - missing!") << endl;
    } else {
        cout << colors::boldGreen(" - found!") << endl;
    }

    cout << "==" << endl;
    tools::printLine();

    if(missing) {
        exit(1);
    }
}

void tools::printHelp() {
    tools::printLine();
    cout << "==" << endl;
    cout << "== plex-yt-dlp [OPTIONS]" << endl;
    cout << "==" << endl;
    cout << "== -h, --help" << endl;
    cout << "==\tDisplay this help screen" << endl;
    cout << "==" << endl;
    cout << "== -v, --version" << endl;
    cout << "==\tDisplay current version" << endl;
    cout << "==" << endl;
    cout << "== -u, --url [urlHere]" << endl;
    cout << "==\tUrl to download" << endl;
    cout << "==" << endl;
    cout << "== -l, --links [pathToTxtFile]" << endl;
    cout << "==\tFile with URLs/Playlists to be download" << endl;
    cout << "==" << endl;
    cout << "== -t, --type [mediaType]" << endl;
    cout << "==\tWhat output you want -- \"video\" or \"music\" or \"rss\"" << endl;
    cout << "==" << endl;
    cout << "== -r, --reverse" << endl;
    cout << "==\tReverse the order of the given playlist or rss feed" << endl;
    cout << "==" << endl;
    cout << "== -io, --indexOverwrite [num] -- optional" << endl;
    cout << "==\tOverwrite the playlist index" << endl;
    cout << "==" << endl;
    cout << "== -ido, --idOverwrite [num] -- optional" << endl;
    cout << "==\tOverwrite the auto ID" << endl;
    cout << "==" << endl;
    tools::printLine();
}

smatch tools::getRegexMatches(string str, string pattern) {
    smatch matches;
    regex_search(str, matches, regex(pattern));
    return matches;
}

void tools::printVersion() {
    cout << "Version is: " << CUR_VERSION << endl;
}