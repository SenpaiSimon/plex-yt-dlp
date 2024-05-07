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
    dl.start();

    return 0;
}