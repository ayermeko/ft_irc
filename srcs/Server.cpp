#include "../includes/Server.hpp"

Server::Server(const std::string &port, const std::string &pass) 
    : _host("127.0.0.1"), _port(port), _pass(pass)
{
    _socket = socket_create();
}

void Server::start()
{
    
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