#pragma once

#include <string>

class Channel
{
private:
    std::string _name;
public:
    Channel(/* args */);
    ~Channel();
    std::string getName();
};
