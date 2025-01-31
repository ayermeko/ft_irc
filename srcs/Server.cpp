#include "Server.hpp"

Server::Server(const std::string &port, const std::string &pass) 
    : _host("127.0.0.1"), _port(port), _pass(pass)
{
    _server_started = true;
    _socket = socket_create();
}

void Server::start()
{
    // poll is used to connect multiple socket fds, for one and more numbers of clients
    pollfd srv;
    srv.fd = _socket;
    srv.events = POLLIN;
    srv.revents = 0;

    _pfds.push_back(srv);
    //TODO: maybe to have the message to display that the server is listening...
    while (_server_started)
    {
        // TODO: fix, may occur some issues here
        if (poll(&_pfds[0], _pfds.size(), -1) < 0)
            throw std::runtime_error("Error while polling from fd!");
        // if the event has occured handle it
        for (pfd_iterator it = _pfds.begin(); it != _pfds.end(); it++)
        {
            if (it->revents == 0)
                continue;
            if ((it->revents & POLLHUP) == POLLHUP)
            {
                this->disconnect_handle(it->fd);
                break;
            }
            if ((it->revents & POLLIN) == POLLIN)
            {
                if (it->fd == _socket)
                {
                    this->client_accept();
                    break;
                }
                this->client_message(it->fd);
            }
        }
        
    }
    
}

void            Server::client_message(int fd)
{
    
}

void            Server::client_accept()
{
    int fd;
    sockaddr_in addr;
    socklen_t size = sizeof(addr);
    if ((fd = accept(_socket, (sockaddr *)&addr, &size)) < 0) // TODO: my not be compatble with C++98
        throw std::runtime_error("Error while accepting a new client!");
    
}

void Server::disconnect_handle(int fd)
{
    (void)fd;
}

int Server::socket_create()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        throw (std::runtime_error("Socket creation faild!"));
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
        throw std::runtime_error("Error: faild to make the port reuseable option!");
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK))
        throw std::runtime_error("Error: while setting socket to NON-BLOCKING!");

    struct sockaddr_in  serv_addr = {}; // to set all to NULL.
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(_port.c_str()));

    if (bind(sockfd, (sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        throw std::runtime_error("Error while binding a socket!");
    if (listen(sockfd, MAX_CONNS) == -1)
        throw std::runtime_error("Error while listening on a socket!");
    return (sockfd);
}