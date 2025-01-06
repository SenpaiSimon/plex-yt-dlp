#include <main.hpp>

int main(int argc, char** argv){
    // info objects
    settings setting;
    json conf;

    // parase argumments
    argumentParser argParser(argc, argv);
    setting = argParser.parse();

    // help and version screen
    if(setting.showHelp){
        tools::printHelp();
        return 0;
    }
    if(setting.showVersion){
        tools::printVersion();
        return 0;
    }

    // check arugment validity
    argParser.checkArguments();

    // check requirements
    tools::checkRequirements();

    // check config file
    conf = config::parseConfig(setting.cwd);

    // create unique ids
    idExtractor extractor(&setting, conf);
    extractor.extractData();

    // start the downloader
    downloader dl(setting, conf);

    if(setting.list) {
        std::ifstream file(setting.linkList);
        string curUrl;
        while(std::getline(file, curUrl)) {
            dl.setNewDlUrl(curUrl);
            dl.start();
        }
        if(dl.getDownloadedCount() <= 0) {
            tools::printEmptyFile();
        }

    } else {
        dl.start();
    }

    // the custom path is set
    if(dl.getCustomFolder() != "") {
        // get the user custom path back
        setting.playlistPath = string(conf["videoPath"]) + "/" + dl.getCustomFolder();
    }

    // post processing for nfo in jellyfin
    if(setting.jellyfin) {
        postProcess post(setting);
        post.convert();
    }

    return 0;
}