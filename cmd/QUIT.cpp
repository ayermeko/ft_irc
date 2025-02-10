#include "Server.hpp"

void FindQ(std::string cmd, std::string tofind, std::string &str)
{
	size_t i = 0;
	for (; i < cmd.size(); i++){
		if (cmd[i] != ' '){
			std::string tmp;
			for (; i < cmd.size() && cmd[i] != ' '; i++)
				tmp += cmd[i];
			if (tmp == tofind)
				break;
			else tmp.clear();
		}
	}
	if (i < cmd.size())
		str = cmd.substr(i);
	i = 0;
	for (; i < str.size() && str[i] == ' '; i++);
	str = str.substr(i);
}

std::string	SplitQuit(std::string cmd)
{
	std::istringstream stm(cmd);
	std::string reason,str;
	stm >> str;
	FindQ(cmd, str, reason);
	if (reason.empty())
		return std::string("Quit");
	if (reason[0] != ':'){ //if the message does not start with ':'
		for (size_t i = 0; i < reason.size(); i++){
			if (reason[i] == ' ')
				{reason.erase(reason.begin() + i, reason.end());break;}
		}
		reason.insert(reason.begin(), ':');
	}
	return reason;
}

void Server::QUIT(std::string cmd, int fd)
{
	std::string reason;
	reason = SplitQuit(cmd);
    size_t i = 0;
	for (; i < _channels.size(); i++)
	{
		if (_channels[i].get_client(fd)){
			_channels[i].remove_client(fd);
			if (_channels[i].getClientsNumber() == 0)
				_channels.erase(_channels.begin() + i);
			else{
				std::string rpl = ":" + getClient(fd)->getNickName() + "!~" + getClient(fd)->getUserName() + "@localhost QUIT " + reason + "\r\n";
				_channels[i].sendTo_all(rpl);
			}
		}
		else if (_channels[i].get_admin(fd)){
			_channels[i].remove_admin(fd);
			if (_channels[i].getClientsNumber() == 0)
				_channels.erase(_channels.begin() + i);
			else{
				std::string rpl = ":" + getClient(fd)->getNickName() + "!~" + getClient(fd)->getUserName() + "@localhost QUIT " + reason + "\r\n";
				_channels[i].sendTo_all(rpl);
			}
		}
	}
    log(YELLOW, "Client <" + my_itos(_clients[i].getFd()) + "> is disconnected!");
	rmChannels(fd);
	removeClient(fd);
	removeFds(fd);
	close(fd);
}