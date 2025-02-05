#pragma once

#include "Colors.hpp"
#include "messanges.hpp"

class Client
{
    private:
        std::string             _nickname;
        std::string             _username;
        int                     _fd;
        bool                    _isOperator;
        bool                    _registered;
        std::string             _buffer;
        std::string             _idaddress;
        bool                    _logedin;
        std::vector<std::string> ChannelsInvite;
    public:
        /* Constructors */
        Client();
        Client(std::string nickname, std::string username, int fd);
        ~Client();
        Client(const Client &src);
        Client      &operator=(const Client &src);
        /* Gettors */
        int         getFd() const;
        bool        getRegistered() const;
        bool        getInviteChannel(std::string &chaname) const;
        std::string getNickname() const;
        bool        getLogedin() const;
        std::string getUserName() const;
        std::string getBuffer() const;
        std::string getHostname() const;
        /* Setters */
        void        setFd(int fd);
        void        setNickname(std::string& nickname);
        void        setLogedin(bool value);
        void        setUsername(std::string& username);
        void        setBuffer(std::string recived);
        void        setRegistered(bool value);
        void        setIpAdd(std::string ipadd);
        /* Methods */
        void        clear_buffer();
        void        addchannelInvite(std::string &channelname);
        void        rmchannelInvite(std::string &channelname);
};

