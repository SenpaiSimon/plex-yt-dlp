#pragma once

#include <string>
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <regex>

#include <misc/colors.hpp>

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <curl_easy.h>
#include <curl_form.h>
#include <curl_ios.h>
#include <curl_exception.h>

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;

using curl::curl_easy;
using curl::curl_ios;
using curl::curl_easy_exception;
using curl::curlcpp_traceback;

using std::regex;
using std::smatch;
using std::regex_search;

namespace fs = std::filesystem;

class tools {
    private:
        tools();
        
    public:
        static bool fileExists(string filePath);
        static string getRequest(string req);
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

        static smatch getRegexMatches(string str, string pattern);

        static void printHelp();
        static void printVersion();
};