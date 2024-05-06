#include <main.hpp>

int main(int argc, char** argv){
    // parase argumments
    argumentParser argParser(argc, argv);
    argParser.parse();

    if(argParser.setting.showHelp){
        tools::printHelp();
        return 0;
    }
    if(argParser.setting.showVersion){
        tools::printVersion();
        return 0;
    }

    // check arugment validity
    argParser.checkArguments();

    tools::checkRequirements();

    return 0;
}