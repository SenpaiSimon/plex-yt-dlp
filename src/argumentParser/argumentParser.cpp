#include <argumentParser/argumentParser.hpp>

argumentParser::argumentParser(int argc, char** argv) {
    this->argCount = argc;
    this->argValues = argv;
}

settings argumentParser::parse() {
    if(this->argCount <= 1) {
        this->setting.showHelp = true;
        return this->setting;
    }

    // parse the rest
    for(int i = 1; i < this->argCount; i++) {
        if(string(this->argValues[i]) == "--help" || string(this->argValues[i]) == "-h") {
            this->setting.showHelp = true;
            continue;
        }

        if(string(this->argValues[i]) == "--version" || string(this->argValues[i]) == "-v") {
            this->setting.showVersion = true;
            continue;
        }

        if(string(this->argValues[i]) == "--url" || string(this->argValues[i]) == "-u") {
            this->setting.dlUrl = string(this->argValues[i + 1]);
            i++;
            continue;
        }

        if(string(this->argValues[i]) == "--type" || string(this->argValues[i]) == "-t") {
            this->setting.mediaType = string(this->argValues[i + 1]);
            i++;
            continue;
        }

        if(string(this->argValues[i]) == "--indexOverwrite" || string(this->argValues[i]) == "-io") {
            this->setting.indexOverwrite = std::stoi(this->argValues[i + 1]);
            i++;
            continue;
        }

        if(string(this->argValues[i]) == "--idOverwrite" || string(this->argValues[i]) == "-ido") {
            this->setting.idOverwrite = std::stoi(this->argValues[i + 1]);
            i++;
            continue;
        }

        cout << colors::boldRed("== Error: Unknown argument: ") << this->argValues[i] << endl;
        exit(1);
    }

    return this->setting;
}

void argumentParser::checkArguments() {
    // check if url has valid url pattern using regex
    string urlPattern = "((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)";
    std::regex urlRegex(urlPattern);

    if (std::regex_match(this->setting.dlUrl, urlRegex) != true) {
        cout << colors::boldRed("== Error: Invalid URL") << endl;
        exit(1);
    }

    // check if mediaType is valid
    if(this->setting.mediaType != "single" && \
       this->setting.mediaType != "music" && \
       this->setting.mediaType != "playlist" && \
       this->setting.mediaType != "rss") {
        cout << colors::boldRed("== Error: Unknown mediaType") << endl;
        exit(1);
    }
}