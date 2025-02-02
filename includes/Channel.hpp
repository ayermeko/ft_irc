#pragma once

class Channel;

#include "Client.hpp"
#include "Server.hpp"
#include "messages.hpp"

class Channel
{
    typedef std::vector<Client *>::iterator client_iterator;

    private:
        std::string             _name;
        Client*                 _admin;
        std::vector<Client *>   _clients;

        /* Modes */
        std::string             _k; // channel key
        size_t                  _l; // limit of channel members
        bool                    _n; // yes/no external messages

        Channel();
        Channel(const Channel& src);
    public:
        Channel(const std::string &name, const std::string &key, Client* admin);
        ~Channel();

        std::string                 getName() const;
        Client*                     getAdmin() const;
                
        std::string                 getKey() const;
        size_t                      getLimit() const;
        bool                        extMsg() const;

        size_t                      getSize() const;
        std::vector<std::string>    getNicknames();
        void                        broadcast(const std::string& message);
};
