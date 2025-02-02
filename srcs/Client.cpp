#include "Client.hpp"

Client::Client(int fd, int port, const std::string &hostname) :
    _fd(fd), _port(port), _hostname(hostname), _state(HANDSHAKE) {}

int         Client::getFd()       const {return _fd;}
int         Client::getPort()     const {return _port;}
std::string Client::getNickname() const {return _nickname;}
std::string Client::getUsername() const {return _username;}
std::string Client::getRealname() const {return _realname;}
std::string Client::getHostname() const {return _hostname;}

void            Client::leave()
{

}

Client::~Client() {}
