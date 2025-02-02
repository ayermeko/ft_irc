#pragma once

#include <sys/socket.h>
#include <sys/poll.h>

#include <string>
#include "messages.hpp"
#include "Channel.hpp"
#include <vector>

#define HANDSHAKE   0
#define LOGIN       1
#define REGISTERED  2
#define DISCONNECED 3

class Client
{
    private:
        int _fd;
        int _port;

        std::string     _nickname;
        std::string     _username;
        std::string     _realname;
        std::string     _hostname;
        int             _state;

        Channel*        _channel;
        
        Client();
        Client(const Client &src);
    public:
        Client(int fd, int port, const std::string &hostname);

        //Getters
        int         getFd() const;
        int         getPort() const;
        std::string getNickname() const;
        std::string getUsername() const;
        std::string getRealname() const;
        std::string getHostname() const;
        int         getPort() const;
        void        leave();
        void        write(const std::string& message) const;
        std::string getPrefix() const;

        ~Client();
};
