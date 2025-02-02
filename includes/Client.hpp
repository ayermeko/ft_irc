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
        ~Client();

        //Getters
        int             getFd() const;
        int             getPort() const;
        std::string     getNickname() const;
        std::string     getUsername() const;
        std::string     getRealname() const;
        std::string     getHostname() const;
        Channel*        getChannel() const;
        std::string     getPrefix() const;

        //Setters
        void            setNickname(const std::string &nickname);
        void            setUsername(const std::string &username);
        void            setRealname(const std::string &realname);
        void            setState(int state);
        void            setChannel(Channel *channel);

        bool            is_registered() const;

        // I/O actions
        void            write(const std::string& message) const;
        void            leave();

};
