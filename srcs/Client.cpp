#include "Client.hpp"

Client::Client(int fd, int port, const std::string &hostname) :
    _fd(fd), _port(port), _hostname(hostname) {}

std::string     Client::getHostname() const {return _hostname;}
int             Client::getPort() const {return _port;}

// Client::~Client() {}