#include "Server.hpp"

Server::Server(const std::string &port, const std::string &pass) 
    : _host("127.0.0.1"), _port(port), _pass(pass)
{
    _server_started = true;
    _socket = socket_create();
    _parser = new Parser(this);
    log("Server is starting...");
}

Server::~Server()
{
}

void Server::start()
{
    // poll is used to connect multiple socket fds, for one and more numbers of clients
    pollfd srv;
    srv.fd = _socket;
    srv.events = POLLIN;
    srv.revents = 0;

    _pfds.push_back(srv);
    log("Server is listening...");
    //TODO: maybe to have the message to display that the server is listening...
    while (_server_started)
    {
        if (poll(&_pfds[0], _pfds.size(), -1) < 0) // TODO: fix, may occur some issues here
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

static std::string     read_message(int fd)
{
    std::string message;

    char buffer[100];
    bzero(buffer, 100);
    while (!strstr(buffer, "\n"))
    {
        bzero(buffer, 100);

        if ((recv(fd, buffer, 100, 0) < 0) && (errno != EWOULDBLOCK))
            throw std::runtime_error("Error while reading buffer from a client!");
        message.append(buffer);
    }
    return (message);
}

void            Server::client_message(int fd)
{
    try
    {
        Client*     client = _clients.at(fd);
        std::string message = read_message(fd);

        // TODO: Invent invoke command for the irssi server
    }
    catch(const std::exception& e)
    {
        std::cout << "Error while handling the client message! " << e.what() << std::endl;
    }
    
}

void            Server::client_accept()
{
    int fd;
    sockaddr_in addr;
    socklen_t size = sizeof(addr);
    if ((fd = accept(_socket, (sockaddr *)&addr, &size)) < 0) // TODO: my not be compatble with C++98
        throw std::runtime_error("Error while accepting a new client!");
    // adding the new client fd in the poll
    pollfd pfd;
    pfd.fd = fd; // new fd form the accept command
    pfd.events = POLLIN;
    _pfds.push_back(pfd);

    char hostname_buffer[NI_MAXHOST];
    int res = getnameinfo((struct sockaddr *) &addr, sizeof(addr), hostname_buffer, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
    if (res != 0)
        throw std::runtime_error("Error: while getting a hostname on a new client!");
    Client* client = new Client(fd, ntohs(addr.sin_port), hostname_buffer);
    _clients.insert(std::make_pair(fd, client));

    // Construct the message using string concatenation
    std::string message = client->getHostname() + ":" + std::to_string(client->getPort()) + " has connected.";
    log(message);
}

void Server::disconnect_handle(int fd)
{
    try
    {
        Client* client = _clients.at(fd);

        client->leave();
    }
    catch(const std::exception& e)
    {
        std::cout << "Error while disconnecting! " << e.what() << std::endl;
    }
}

int Server::socket_create()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
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