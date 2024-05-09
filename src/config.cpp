#include <config.hpp>

json config::parseConfig(string cwd) {
    json conf;

    if(!tools::fileExists(cwd + "/config.json")) {
        conf = {
            {"videoPath", "./video"},
            {"rssPath", "./rss"},
            {"musicPath", "./music"},
            {"tempPath", "./temp"}
        };
        std::ofstream o("config.json");
        o << std::setw(4) << conf << std::endl;

        cout << "==" << endl;
        cout << "==" <<colors::yellow(" Created \"") << colors::boldGreen("config.json") << colors::yellow("\" in current working directory.") << endl;
        cout << "=="<< colors::yellow(" Please fill in the fields in the config.json file.") << endl;
        cout << "==" << endl;
        tools::printLine();
        exit(0);
    } else {
        std::ifstream i(cwd + "/config.json");
        i >> conf;

        return conf;
    }
}