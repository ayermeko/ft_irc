#include "Client.hpp"

Client::Client(int fd, int port, const std::string &hostname) :
    _fd(fd), _port(port), _hostname(hostname), _state(HANDSHAKE) {}

int         Client::getFd()       const {return _fd;}
int         Client::getPort()     const {return _port;}
std::string Client::getNickname() const {return _nickname;}
std::string Client::getUsername() const {return _username;}
std::string Client::getRealname() const {return _realname;}
std::string Client::getHostname() const {return _hostname;}

std::string     Client::getPrefix() const 
{
    std::string username = _username.empty() ? "" : "!" + _username;
    std::string hostname = _hostname.empty() ? "" : "@" + _hostname;

    return _nickname + username + hostname;
}

void            Client::leave()
{
    if (!_channel)
        return ;
    const std::string name = _channel->getName();
    _channel->broadcast(RPL_PART(getPrefix(), _channel->getName()));

}

void            Client::write(const std::string& message) const
{
    std::string buffer = message + "\r\n";
    if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
        throw std::runtime_error("Error while sending a message to a client!");
}

Client::~Client() {}
