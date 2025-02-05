#pragma once

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

class Channel;
class Client;
class Server;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"


#include <ctime>
#include <sstream>
#include <iostream>
#include <string>

static inline void log(const std::string color, const std::string& message) {
    // Get the current time and format it
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    // Output the formatted log message with timestamp
    std::cout   << "\033[0;34m[" << buffer << "]\033[0m " 
                << color << message << "\033[0m " <<std::endl;
}

static inline std::string my_itos(int num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}