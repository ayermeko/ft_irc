#include "Server.hpp"

int Server::SplitJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd)
{
	std::vector<std::string> tmp;
	std::string ChStr, PassStr, buff;
	std::istringstream iss(cmd);
	while(iss >> cmd)
		tmp.push_back(cmd);
	if (tmp.size() < 2) {token.clear(); return 0;}
	tmp.erase(tmp.begin());
	ChStr = tmp[0]; tmp.erase(tmp.begin());
	if (!tmp.empty()) {PassStr = tmp[0]; tmp.clear();}
	for (size_t i = 0; i < ChStr.size(); i++){
		if (ChStr[i] == ',')
				{token.push_back(std::make_pair(buff, "")); buff.clear();}
		else buff += ChStr[i];
	}
	token.push_back(std::make_pair(buff, ""));
	if (!PassStr.empty()){
		size_t j = 0; buff.clear();
		for (size_t i = 0; i < PassStr.size(); i++){
			if (PassStr[i] == ',')
				{token[j].second = buff; j++; buff.clear();}
			else buff += PassStr[i];
		}
		token[j].second = buff;
	}
	for (size_t i = 0; i < token.size(); i++)//erase the empty channel names
		{if (token[i].first.empty())token.erase(token.begin() + i--);}
	for (size_t i = 0; i < token.size(); i++){//ERR_NOSUCHCHANNEL (403) // if the channel doesn't exist
		if (*(token[i].first.begin()) != '#')
			{senderror(403, getClient(fd)->getNickName(), token[i].first, getClient(fd)->getFd(), " :No such channel\r\n"); token.erase(token.begin() + i--);}
		else
			token[i].first.erase(token[i].first.begin());
	}
	return 1;
}

int Server::SearchForClients(std::string nickname)
{
	int count = 0;
	for (size_t i = 0; i < this->_channels.size(); i++){
		if (this->_channels[i].GetClientInChannel(nickname))
			count++;
	}
	return count;
}

bool IsInvited(Client *cli, std::string ChName, int flag){
	if(cli->getInviteChannel(ChName)){
		if (flag == 1)
			cli->RmChannelInvite(ChName);
		return true;
	}
	return false;
}

void Server::ExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd)
{
	if (this->_channels[j].GetClientInChannel(getClient(fd)->getNickName()))// if the client is already registered
		return;
	if (SearchForClients(getClient(fd)->getNickName()) >= 10)//ERR_TOOMANYCHANNELS (405) // if the client is already in 10 channels
		{senderror(405, getClient(fd)->getNickName(), getClient(fd)->getFd(), " :You have joined too many channels\r\n"); return;}
	if (!this->_channels[j].getPassword().empty() && this->_channels[j].getPassword() != token[i].second){// ERR_BADCHANNELKEY (475) // if the password is incorrect
		if (!IsInvited(getClient(fd), token[i].first, 0))
			{senderror(475, getClient(fd)->getNickName(), "#" + token[i].first, getClient(fd)->getFd(), " :Cannot join channel (+k) - bad key\r\n"); return;}
	}
	if (this->_channels[j].getInvitOnly()){// ERR_INVITEONLYCHAN (473) // if the channel is invit only
		if (!IsInvited(getClient(fd), token[i].first, 1))
			{senderror(473, getClient(fd)->getNickName(), "#" + token[i].first, getClient(fd)->getFd(), " :Cannot join channel (+i)\r\n"); return;}
	}
	if (this->_channels[j].getLimit() && this->_channels[j].getClientsNumber() >= this->_channels[j].getLimit())// ERR_CHANNELISFULL (471) // if the channel reached the limit of number of clients
		{senderror(471, getClient(fd)->getNickName(), "#" + token[i].first, getClient(fd)->getFd(), " :Cannot join channel (+l)\r\n"); return;}
	// add the client to the channel
	Client *cli = getClient(fd);
	this->_channels[j].add_client(*cli);
	if(_channels[j].getTopicName().empty())
		_sendResponse(RPL_JOINMSG(getClient(fd)->getHostname(),getClient(fd)->getIpAdd(),token[i].first) + \
			RPL_NAMREPLY(getClient(fd)->getNickName(),_channels[j].getName(),_channels[j].clientChannel_list()) + \
			RPL_ENDOFNAMES(getClient(fd)->getNickName(),_channels[j].getName()),fd);
	else
		_sendResponse(RPL_JOINMSG(getClient(fd)->getHostname(),getClient(fd)->getIpAdd(),token[i].first) + \
			RPL_TOPICIS(getClient(fd)->getNickName(),_channels[j].getName(),_channels[j].getTopicName()) + \
			RPL_NAMREPLY(getClient(fd)->getNickName(),_channels[j].getName(),_channels[j].clientChannel_list()) + \
			RPL_ENDOFNAMES(getClient(fd)->getNickName(),_channels[j].getName()),fd);
    _channels[j].sendTo_all(RPL_JOINMSG(getClient(fd)->getHostname(),getClient(fd)->getIpAdd(),token[i].first), fd);
}


void Server::NotExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int fd)
{
	if (SearchForClients(getClient(fd)->getNickName()) >= 10)//ERR_TOOMANYCHANNELS (405) // if the client is already in 10 channels
		{senderror(405, getClient(fd)->getNickName(), getClient(fd)->getFd(), " :You have joined too many channels\r\n"); return;}
	Channel newChannel;
	newChannel.setName(token[i].first);
	newChannel.add_admin(*getClient(fd));
	newChannel.set_createiontime();
	this->_channels.push_back(newChannel);
	// notifiy thet the client joined the channel
    _sendResponse(RPL_JOINMSG(getClient(fd)->getHostname(),getClient(fd)->getIpAdd(),newChannel.getName()) + \
        RPL_NAMREPLY(getClient(fd)->getNickName(),newChannel.getName(),newChannel.clientChannel_list()) + \
        RPL_ENDOFNAMES(getClient(fd)->getNickName(),newChannel.getName()),fd);
}

void Server::JOIN(std::string cmd, int fd)
{
	std::vector<std::pair<std::string, std::string> > token;
	// SplitJoin(token, cmd, fd);
	if (!SplitJoin(token, cmd, fd))// ERR_NEEDMOREPARAMS (461) // if the channel name is empty
		{senderror(461, getClient(fd)->getNickName(), getClient(fd)->getFd(), " :Not enough parameters\r\n"); return;}
	if (token.size() > 10) //ERR_TOOMANYTARGETS (407) // if more than 10 Channels
		{senderror(407, getClient(fd)->getNickName(), getClient(fd)->getFd(), " :Too many channels\r\n"); return;}
	for (size_t i = 0; i < token.size(); i++){
		bool flag = false;
		for (size_t j = 0; j < this->_channels.size(); j++){
			if (this->_channels[j].getName() == token[i].first){
				ExistCh(token, i, j, fd);
				flag = true; break;
			}
		}
		if (!flag)
			NotExistCh(token, i, fd);
	}
}