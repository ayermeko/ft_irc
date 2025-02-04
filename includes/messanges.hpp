#pragma once

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

class Channel;
class Client;
class Server;

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

static inline void log(const std::string& color, const std::string& message) 
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::cout << "\033[0;34m[" 
              << std::put_time(std::localtime(&now_c), "%d-%m-%Y %H:%M:%S") 
              << "]\033[0m " 
              << color << message << "\033[0m" << std::endl;
}