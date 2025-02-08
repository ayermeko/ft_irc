#include "Server.hpp"

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
            
        }
    }
}