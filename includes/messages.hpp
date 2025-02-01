#pragma once

#include <ctime>
#include <sstream>
#include <iostream>
#include <string>

static inline void log(const std::string& message) {
    // Get the current time and format it
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);

    // Output the formatted log message with timestamp
    std::cout << "\033[0;34m[" << buffer << "]\033[0m " << message << std::endl;
}
