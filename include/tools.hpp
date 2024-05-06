#pragma once

#include <string>
#include <iostream>
#include <unistd.h>

#include <colors.hpp>

using std::string;
using std::cout;
using std::endl;


class tools {
    private:
        tools();
        
    public:
        // prints a line
        static void printLine();
        // checks if a file is executable
        static bool checkIfIsExecutable(const string& name);
        // get the path of a command
        static string getExecutablePath(const string& name);
        // execute command and get the output
        static string executeCommand(const string& command);
        // check all requirements for the project
        static void checkRequirements();

        static void printHelp();
        static void printVersion();
};