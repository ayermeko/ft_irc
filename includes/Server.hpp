#pragma once

#include <vector>
#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h> // inet_ntoa()
#include <netdb.h> // gethostname()...
#include <poll.h>
#include <map>
#include <cstring> 
#include <unistd.h> // close()
#include <fcntl.h>
#include <algorithm>

#include <stdexcept>
#include <cerrno>
#include <string>

#include "Client.hpp"

#define MAX_CONNS 192

class Server
{
    private:
        typedef std::vector<pollfd>::iterator       pfd_iterator;

        const std::string _port;
        const std::string _pass;
        const std::string _host;
        int _socket;
        std::vector<pollfd> _pfds;
        bool _server_started;
    public:
        Server(const std::string &port, const std::string &pass);
        //TODO: Ortodox Conon.
        int socket_create();
        void start();
        void disconnect_handle(int fd);
        void client_connect();
};
