#include "Server.hpp"

void Server::client_authen(int fd, std::string cmd)
{
	Client *cli = getClient(fd);
	cmd = cmd.substr(4);
	size_t pos = cmd.find_first_not_of("\t\v ");
	if(pos < cmd.size())
	{
		cmd = cmd.substr(pos);
		if(cmd[0] == ':')
			cmd.erase(cmd.begin());
	}
	if(pos == std::string::npos || cmd.empty()) 
		_sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
	else if(!cli->getRegistered())
	{
		std::string pass = cmd;
		if(pass == _password)
			cli->setRegistered(true);
		else
            _sendResponse(ERR_INCORPASS(std::string("*")), fd);
	}
	else
        _sendResponse(ERR_ALREADYREGISTERED(getClient(fd)->getNickName()), fd);
}

bool Server::is_validNickname(std::string& nickname)
{
		
	if(!nickname.empty() && (nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':'))
		return false;
	for(size_t i = 1; i < nickname.size(); i++)
	{
		if(!std::isalnum(nickname[i]) && nickname[i] != '_')
			return false;
	}
	return true;
}

bool Server::nickNameInUse(std::string& nickname)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getNickName() == nickname)
			return (true);
	}
	return (false);
}


void Server::set_nickname(std::string cmd, int fd)
{
    std::string inuse;
	cmd = cmd.substr(4);
	size_t pos = cmd.find_first_not_of("\t\v ");
    if(pos < cmd.size())
	{
		cmd = cmd.substr(pos);
		if(cmd[0] == ':')
			cmd.erase(cmd.begin());
	}
    Client *client = getClient(fd);
    if(pos == std::string::npos || cmd.empty())
	{
        _sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd); 
        return ;
    }
    if (nickNameInUse(cmd) && client->getNickName() != cmd)
    {
        inuse = "*";
		if(client->getNickName().empty())
			client->setNickname(inuse);
	    _sendResponse(ERR_NICKINUSE(std::string(cmd)), fd); 
		return ;
    }
    if(!is_validNickname(cmd)) {
		_sendResponse(ERR_ERRONEUSNICK(std::string(cmd)), fd);
		return;
	}
    else
    {
        if(client && client->getRegistered())
        {
            std::string oldnick = client->getNickName();
            client->setNickname(cmd);
            for (size_t i = 0; i < _channels.size(); i++)
            {
                Client *cli = _channels[i].GetClientInChannel(oldnick);
                if (cli)
                    cli->setNickname(cmd);
            }
            if(!oldnick.empty() && oldnick != cmd)
			{
                if(oldnick == "*" && !client->getUserName().empty())
				{
					client->setLogedin(true);
					_sendResponse(RPL_CONNECTED(client->getNickName()), fd);
					_sendResponse(RPL_NICKCHANGE(client->getNickName(),cmd), fd);
				}
				else
					_sendResponse(RPL_NICKCHANGE(oldnick,cmd), fd);
				return;
            }
        }
    }
    if(client && client->getRegistered() && !client->getUserName().empty() && !client->getNickName().empty() && client->getNickName() != "*" && !client->getLogedIn())
	{
		client->setLogedin(true);
		_sendResponse(RPL_CONNECTED(client->getNickName()), fd);
	}
}


void	Server::set_username(std::string& cmd, int fd)
{
	std::vector<std::string> splited_cmd = split_cmd(cmd);

	Client *cli = getClient(fd); 
	if((cli && splited_cmd.size() < 5))
		{_sendResponse(ERR_NOTENOUGHPARAM(cli->getNickName()), fd); return; }
	if(!cli  || !cli->getRegistered())
		_sendResponse(ERR_NOTREGISTERED(std::string("*")), fd);
	else if (cli && !cli->getUserName().empty())
		{_sendResponse(ERR_ALREADYREGISTERED(cli->getNickName()), fd); return;}
	else
		cli->setUsername(splited_cmd[1]);
	if(cli && cli->getRegistered() && !cli->getUserName().empty() && !cli->getNickName().empty() && cli->getNickName() != "*"  && !cli->getLogedIn())
	{
		cli->setLogedin(true);
		_sendResponse(RPL_CONNECTED(cli->getNickName()), fd);
	}
}