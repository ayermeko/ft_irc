#include "Server.hpp"

void FindPR(std::string cmd, std::string tofind, std::string &str)
{
	size_t i = 0;
	for (; i < cmd.size(); i++){
		if (cmd[i] != ' '){
			std::string tmp;
			for (; i < cmd.size() && cmd[i] != ' '; i++)
				tmp += cmd[i];
			if (tmp == tofind) break;
			else tmp.clear();
		}
	}
	if (i < cmd.size()) str = cmd.substr(i);
	i = 0;
	for (; i < str.size() && str[i] == ' '; i++);
	str = str.substr(i);
}

std::string SplitCmdPR(std::string &cmd, std::vector<std::string> &tmp)
{
	std::stringstream ss(cmd);
	std::string str, reason;
	int count = 2;
	while (ss >> str && count--)
		tmp.push_back(str);
	if(tmp.size() != 2) return std::string("");
	FindPR(cmd, tmp[1], reason);
	return reason;
}

int Server::SplitCmdPart(std::string cmd, std::vector<std::string> &tmp, std::string &reason, int fd)
{
	reason = SplitCmdPR(cmd, tmp);
	if(tmp.size() < 2) {tmp.clear(); return 0;}
	tmp.erase(tmp.begin());
	std::string str = tmp[0]; std::string str1; tmp.clear();
	for (size_t i = 0; i < str.size(); i++){//split the first string by ',' to get the channels names
		if (str[i] == ',')
			{tmp.push_back(str1); str1.clear();}
		else str1 += str[i];
	}
	tmp.push_back(str1);
	for (size_t i = 0; i < tmp.size(); i++)//erase the empty strings
		{if (tmp[i].empty())tmp.erase(tmp.begin() + i--);}
	if (reason[0] == ':') reason.erase(reason.begin());
	else //shrink to the first space
		{for (size_t i = 0; i < reason.size(); i++){if (reason[i] == ' '){reason = reason.substr(0, i);break;}}}
	for (size_t i = 0; i < tmp.size(); i++){// erase the '#' from the channel name and check if the channel valid
			if (*(tmp[i].begin()) == '#')
				tmp[i].erase(tmp[i].begin());
			else
				{senderror(403, getClient(fd)->getNickName(), tmp[i], getClient(fd)->getFd(), " :No such channel\r\n"); tmp.erase(tmp.begin() + i--);}
		}
	return 1;
}

void Server::PART(std::string cmd, int fd)
{
	std::vector<std::string> tmp;
	std::string reason;
	if (!SplitCmdPart(cmd, tmp, reason, fd))// ERR_NEEDMOREPARAMS (461) // if the channel name is empty
		{senderror(461, getClient(fd)->getNickName(), getClient(fd)->getFd(), " :Not enough parameters\r\n"); return;}
	for (size_t i = 0; i < tmp.size(); i++){
		bool flag = false;
		for (size_t j = 0; j < this->_channels.size(); j++){ // search for the channel
			if (this->_channels[j].getName() == tmp[i]){ // check if the channel exist
				flag = true;
				if (!_channels[j].get_client(fd) && !_channels[j].get_admin(fd)) // ERR_NOTONCHANNEL (442) // if the client is not in the channel
					{senderror(442, getClient(fd)->getNickName(), "#" + tmp[i], getClient(fd)->getFd(), " :You're not on that channel\r\n"); continue;}
					std::stringstream ss;
					ss << ":" << getClient(fd)->getNickName() << "!~" << getClient(fd)->getUserName() << "@" << "localhost" << " PART #" << tmp[i];
					if (!reason.empty())
						ss << " :" << reason << "\r\n";
					else ss << "\r\n";
					_channels[j].sendTo_all(ss.str());
					if (_channels[j].get_admin(_channels[j].GetClientInChannel(getClient(fd)->getNickName())->getFd()))
						_channels[j].remove_admin(_channels[j].GetClientInChannel(getClient(fd)->getNickName())->getFd());
					else
						_channels[j].remove_client(_channels[j].GetClientInChannel(getClient(fd)->getNickName())->getFd());
					if (_channels[j].getClientsNumber() == 0)
						_channels.erase(_channels.begin() + j);
			}
		}
		if (!flag) // ERR_NOSUCHCHANNEL (403) // if the channel doesn't exist
			senderror(403, getClient(fd)->getNickName(), "#" + tmp[i], getClient(fd)->getFd(), " :No such channel\r\n");
	}
}