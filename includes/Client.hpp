#pragma once

#include <sys/socket.h>
#include <sys/poll.h>

#include <string>
#include <vector>

class Client
{
    private:
        int _fd;
        int _port;
        std::string     _hostname;
        Client();
        Client(const Client &src);
    public:
        Client(int fd, int port, const std::string &hostname);
        std::string getHostname() const;
        int         getPort() const;
        ~Client();
};
