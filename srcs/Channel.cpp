#include "Channel.hpp"

Channel::Channel(Channel const &src) {*this = src;}
Channel         &Channel::operator=(Channel const &src)
{
    (void)src;
    return (*this);
}

Channel::Channel(/* args */)
{
}

std::string Channel::getName()
{
    return (_name);
}


Channel::~Channel()
{
}
