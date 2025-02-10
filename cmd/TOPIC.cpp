#include "Server.hpp"

std::string Server::tTopic()
{
	std::time_t current = std::time(NULL);
	std::stringstream res;

	res << current;
	return res.str();
}
std::string Server::gettopic(std::string &input)
{
	size_t pos = input.find(":");
	if (pos == std::string::npos)
	{
		return "";
	}
	return input.substr(pos);
}

int Server::getpos(std::string &cmd)
{
	for (int i = 0; i < (int)cmd.size(); i++)
		if (cmd[i] == ':' && (cmd[i - 1] == 32))
			return i;
	return -1;
}

void Server::TOPIC(std::string &cmd, int &fd)
{
	if (cmd == "TOPIC :")
		{senderror(461, getClient(fd)->getNickName(), fd, " :Not enough parameters\r\n");return;} // ERR_NEEDMOREPARAMS (461) if there are not enough parameters
	std::vector<std::string> scmd = split_cmd(cmd);
	if (scmd.size() == 1)
		{senderror(461, getClient(fd)->getNickName(), fd, " :Not enough parameters\r\n");return;} // ERR_NEEDMOREPARAMS (461) if there are not enough parameters
	std::string nmch = scmd[1].substr(1);
	if (!getChannel(nmch)) // ERR_NOSUCHCHANNEL (403) if the given channel does not exist
		{senderror(403, "#" + nmch, fd, " :No such channel\r\n"); return;}
	if (!(getChannel(nmch)->get_client(fd)) && !(getChannel(nmch)->get_admin(fd)))
		{senderror(442, "#" + nmch, fd, " :You're not on that channel\r\n");return;} // ERR_NOTONCHANNEL (442) if the client is not on the channel
	if (scmd.size() == 2)
	{
		if (getChannel(nmch)->getTopicName() == "")
		{_sendResponse(": 331 " + getClient(fd)->getNickName() + " " + "#" + nmch + " :No topic is set\r\n", fd);return;} // RPL_NOTOPIC (331) if no topic is set
		size_t pos = getChannel(nmch)->getTopicName().find(":");
		if (getChannel(nmch)->getTopicName() != "" && pos == std::string::npos)
		{
			_sendResponse(": 332 " + getClient(fd)->getNickName() + " " + "#" + nmch + " " + getChannel(nmch)->getTopicName() + "\r\n", fd);			  // RPL_TOPIC (332) if the topic is set
			_sendResponse(": 333 " + getClient(fd)->getNickName() + " " + "#" + nmch + " " + getClient(fd)->getNickName() + " " + getChannel(nmch)->getTime() + "\r\n", fd); // RPL_TOPICWHOTIME (333) if the topic is set
			return;
		}
		else
		{
			size_t pos = getChannel(nmch)->getTopicName().find(" ");
			if (pos == 0)
				getChannel(nmch)->getTopicName().erase(0, 1);
			_sendResponse(": 332 " + getClient(fd)->getNickName() + " " + "#" + nmch + " " + getChannel(nmch)->getTopicName() + "\r\n", fd);			  // RPL_TOPIC (332) if the topic is set
			_sendResponse(": 333 " + getClient(fd)->getNickName() + " " + "#" + nmch + " " + getClient(fd)->getNickName() + " " + getChannel(nmch)->getTime() + "\r\n", fd); // RPL_TOPICWHOTIME (333) if the topic is set
			return;
		}
	}

	if (scmd.size() >= 3)
	{
		std::vector<std::string> tmp;
		int pos = getpos(cmd);
		if (pos == -1 || scmd[2][0] != ':')
		{
			tmp.push_back(scmd[0]);
			tmp.push_back(scmd[1]);
			tmp.push_back(scmd[2]);
		}
		else
		{
			tmp.push_back(scmd[0]);
			tmp.push_back(scmd[1]);
			tmp.push_back(cmd.substr(getpos(cmd)));
		}

		if (tmp[2][0] == ':' && tmp[2][1] == '\0')
		{senderror(331, "#" + nmch, fd, " :No topic is set\r\n");return;} // RPL_NOTOPIC (331) if no topic is set

		if (getChannel(nmch)->gettopic_restriction() && getChannel(nmch)->get_client(fd))
		{senderror(482, "#" + nmch, fd, " :You're Not a channel operator\r\n");return;} // ERR_CHANOPRIVSNEEDED (482) if the client is not a channel operator
		else if (getChannel(nmch)->gettopic_restriction() && getChannel(nmch)->get_admin(fd))
		{
			getChannel(nmch)->setTime(tTopic());
			getChannel(nmch)->setTopicName(tmp[2]);
			std::string rpl;
			size_t pos = tmp[2].find(":");
			if (pos == std::string::npos)
				rpl = ":" + getClient(fd)->getNickName() + "!" + getClient(fd)->getUserName() + "@localhost TOPIC #" + nmch + " :" + getChannel(nmch)->getTopicName() + CRLF; // RPL_TOPIC (332) if the topic is set
			else
				rpl = ":" + getClient(fd)->getNickName() + "!" + getClient(fd)->getUserName() + "@localhost TOPIC #" + nmch + " " + getChannel(nmch)->getTopicName() + CRLF; // RPL_TOPIC (332) if the topic is set
			getChannel(nmch)->sendTo_all(rpl);
		}
		else
		{
			std::string rpl;
			size_t pos = tmp[2].find(":");
			if (pos == std::string::npos)
			{
				getChannel(nmch)->setTime(tTopic());
				getChannel(nmch)->setTopicName(tmp[2]);
				rpl = ":" + getClient(fd)->getNickName() + "!" + getClient(fd)->getUserName() + "@localhost TOPIC #" + nmch + " " + getChannel(nmch)->getTopicName() + CRLF; // RPL_TOPIC (332) if the topic is set
			}
			else
			{
				size_t poss = tmp[2].find(" ");
				getChannel(nmch)->setTopicName(tmp[2]);
				if (poss == std::string::npos && tmp[2][0] == ':' && tmp[2][1] != ':')
					tmp[2] = tmp[2].substr(1);
				getChannel(nmch)->setTopicName(tmp[2]);
				getChannel(nmch)->setTime(tTopic());
				rpl = ":" + getClient(fd)->getNickName() + "!" + getClient(fd)->getUserName() + "@localhost TOPIC #" + nmch + " " + getChannel(nmch)->getTopicName() + CRLF; // RPL_TOPIC (332) if the topic is set
			}
			getChannel(nmch)->sendTo_all(rpl);
		}
	}
}