#include <misc/colors.hpp>

string colors::red(string text) {
    return RED + text + RESET;
}

string colors::green(string text) {
    return GREEN + text + RESET;
}

string colors::yellow(string text) {
    return YELLOW + text + RESET;
}

string colors::blue(string text) {
    return BLUE + text + RESET;
}

string colors::magenta(string text) {
    return MAGENTA + text + RESET;
}

string colors::cyan(string text) {
    return CYAN + text + RESET;
}

string colors::white(string text) {
    return WHITE + text + RESET;
}

string colors::boldRed(string text) {
    return BOLDRED + text + RESET;
}

string colors::boldGreen(string text) {
    return BOLDGREEN + text + RESET;
}

string colors::boldYellow(string text) {
    return BOLDYELLOW + text + RESET;
}

string colors::boldBlue(string text) {
    return BOLDBLUE + text + RESET;
}

string colors::boldMagenta(string text) {
    return BOLDMAGENTA + text + RESET;
}

string colors::boldCyan(string text) {
    return BOLDCYAN + text + RESET;
}

string colors::boldWhite(string text) {
    return BOLDWHITE + text + RESET;
}