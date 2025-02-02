#include "Channel.hpp"

Channel::Channel(const std::string &name, const std::string &key, Client *admin)
        : _name(name), _admin(admin), _k(key), _l(0), _n(false)
{
}

Channel::~Channel() {}

std::string                 Channel::getName() const { return _name; }
Client*                     Channel::getAdmin() const { return _admin; }

std::string                 Channel::getKey() const { return _k; }
size_t                      Channel::getLimit() const { return _l; }
bool                        Channel::extMsg() const { return _n; }

size_t                      Channel::getSize()const { return _clients.size(); }

std::vector<std::string>    Channel::getNicknames()
{
    std::vector<std::string> nicknames;

    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        Client* client = *it_b;

        std::string nick = (client == _admin ? "@" : "") + client->getNickname();
        nicknames.push_back(nick);

        it_b++;
    }

    return nicknames;
}

void                        Channel::broadcast(const std::string& message)
{
    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        (*it_b)->write(message);
        it_b++;
    }
}

