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
#include "Channel.hpp"
#include "Parser.hpp"

class Client;

#define MAX_CONNS 192

class Server
{
    typedef std::vector<pollfd>::iterator       pfd_iterator;
    typedef std::vector<Channel *>::iterator    channel_iterator;
    typedef std::map<int, Client *>::iterator   client_iterator;

    private:
        bool                    _server_started;
        int                     _socket;

        const std::string       _host;
        const std::string       _port;
        const std::string       _pass;

        std::vector<pollfd>     _pfds;
        std::vector<Channel *>  _channels;
        std::map<int, Client *> _clients;

        Parser*                 _parser;

        Server();
        Server(const Server &src);
    public:
        Server(const std::string &port, const std::string &pass);
        ~Server();

        std::string     getPass() const;
        Client*         getClient(const std::string &nickname);
        Channel*        getChannel(const std::string &channelname);

        int             socket_create();
        void            start();
        void            client_accept();
        void            client_message(int fd);
        void            disconnect_handle(int fd);

        Channel*        create_channel(const std::string &name, const std::string &key, Client *client);
};
