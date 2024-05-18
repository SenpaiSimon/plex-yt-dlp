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
#include <unordered_set>

using std::string;
using std::unordered_set;
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
        typedef enum binaryStatus {
            MISSING,
            FOUND,
            EXECUTABLE
        } binaryStatus_t;
        
    public:
        static bool fileExists(string filePath);
        static string getRequest(string req);
        static string escapePath(const string& path);
        // prints a line
        static void printLine();
        // execute command and get the output
        static string executeCommand(const string& command);
        // check all requirements for the project
        static void checkRequirements();

        static smatch getRegexMatches(string str, string pattern);

        static binaryStatus_t checkBinaryStatus(string name);

        static void printHelp();
        static void printVersion();
};