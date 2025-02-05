#include "Server.hpp"

Server::Server(){this->_socket = -1;}
Server::~Server(){}
Server::Server(Server const &src) { *this = src; }
Server                      &Server::operator=(Server const &src)
{
    (void)src;
    return (*this);
}

// getters
int                         Server::getFd() const{return _socket;}
int                         Server::getPort() const{return _port;}
std::string                 Server::getPass() const{return _pass;}
Client*                     Server::getClient(int fd)
{
    for (size_t i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getFd() == fd)
            return &(this->_clients[i]);
    }
    return NULL;
}
Client*                     Server::getClientNick(std::string nickname)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
    {
		if (this->_clients[i].getNickname() == nickname)
			return &(this->_clients[i]);
	}
	return NULL;
}
Channel*                    Server::getChannel(std::string name)
{
	for (size_t i = 0; i < this->_channels.size(); i++){
		if (this->_channels[i].getName() == name)
			return &_channels[i];
	}
	return NULL;
}
// setters
void                        Server::setFd(int socket){_socket = socket;}
void                        Server::setPort(int port){_port = port;}
void                        Server::setPassword(std::string pass){_pass = pass;}
// additional
void                        Server::addClient(Client nclient){_clients.push_back(nclient);}
void                        Server::addChannel(Channel nchannel){_channels.push_back(nchannel);}
void                        Server::addFds(pollfd npfd){_pfds.push_back(npfd);}
// remove&clear
void                        Server::removeClient(int fd)
{
    for (size_t i = 0; i < this->_clients.size(); i++){
		if (this->_clients[i].getFd() == fd)
			{this->_clients.erase(this->_clients.begin() + i); return;}
	}
}
void                        Server::removeChannel(std::string name)
{
    for (size_t i = 0; i < this->_channels.size(); i++){
		if (this->_channels[i].getName() == name)
			{this->_channels.erase(this->_channels.begin() + i); return;}
	}
}
void                        Server::removeFds(int fd)
{
   	for (size_t i = 0; i < this->_pfds.size(); i++){
		if (this->_pfds[i].fd == fd)
			{this->_pfds.erase(this->_pfds.begin() + i); return;}
	}
}
void                        Server::removeChannels(int fd)
{
    (void)fd;
    //TODO: complete the Channels to finish this part
}
// error msgs
void                        Server::sendError(int code, std::string clientname, int fd, std::string msg)
{
    std::stringstream ss;
	ss << ":localhost " << code << " " << clientname << msg;
	std::string resp = ss.str();
	if(send(fd, resp.c_str(), resp.size(),0) == -1)
	    std::cerr << "send() faild" << std::endl;
}
void                        Server::sendError(int code, std::string clientname, std::string channelname, int fd, std::string msg)
{
    std::stringstream ss;
	ss << ":localhost " << code << " " << clientname << " " << channelname << msg;
	std::string resp = ss.str();
	if(send(fd, resp.c_str(), resp.size(),0) == -1)
	    std::cerr << "send() faild" << std::endl;
}
void                        Server::_sendResponse(std::string response, int fd)
{
    if(send(fd, response.c_str(), response.size(), 0) == -1)
	    std::cerr << "Response send() faild" << std::endl;
}
// signal Handle
bool Server::_signal_f = false;
void                 Server::signalHandler(int signum)
{
    (void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
    Server::_signal_f = true;
}
void                        Server::closeFds()
{
	for(size_t i = 0; i < _clients.size(); i++){
        log(RED, "Client <" + my_itos(_clients[i].getFd()) + "> Disconnected");
        close(_clients[i].getFd());
    }
	if (_socket != -1){
        log(RED, "Server <" + my_itos(_socket) + "> Disconnected");
        close(_socket);
    }
}
// server methods
void                        Server::init(int port, std::string pass)
{
    this->_pass = pass;
    this->_port = port;

    log(GREEN ,"Server started, waiting for the connections...");
    while (Server::_signal_f == false)
    {
        if ((poll(&_pfds[0], _pfds.size(), -1) == -1) && Server::_signal_f == false)
            throw(std::runtime_error("poll() faild"));
        for (size_t i = 0; i < _pfds.size(); i++)
        {
            if (_pfds[i].revents & POLLIN)
            {
                if (_pfds[i].fd == _socket)
                    this->acceptClient();
                else
                    this->reciveNewData(_pfds[i].fd);                
            }
        }
    }
    closeFds();    
}
void                        Server::acceptClient()
{
    Client newclient;
    memset(&_cadd, 0, sizeof(_cadd));
    socklen_t len = sizeof(_cadd);
    int incofd = accept(_socket, (sockaddr *)&(_cadd), &len);
    if (incofd == -1)
		{std::cout << "accept() failed" << std::endl; return;}
    if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1)
		{std::cout << "fcntl() failed" << std::endl; return;}
    _pnclient.fd = incofd;
    _pnclient.events = POLLIN;
    _pnclient.revents = 0;
    newclient.setFd(incofd);
    newclient.setIpAdd(inet_ntoa((_cadd.sin_addr)));
    _clients.push_back(newclient);
    _pfds.push_back(_pnclient);
    log(GREEN, "Client <" + my_itos(incofd) + "> Connected");
}
void                        Server::socket_create()
{
    int en = 1;
    _add.sin_family = AF_INET;
    _add.sin_addr.s_addr = INADDR_ANY;
    _add.sin_port = htons(_port);
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1)
        throw(std::runtime_error("faild to create socket"));
    if ((setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1))
        throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
    if ((fcntl(_socket, F_SETFL, O_NONBLOCK)) == -1)
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
    if (bind(_socket, (struct sockaddr *)&_add, sizeof(_add)) == -1)
		throw(std::runtime_error("faild to bind socket"));
	if (listen(_socket, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() faild"));
    _pnclient.fd = _socket;
    _pnclient.events = POLLIN;
    _pnclient.revents = 0;

    _pfds.push_back(_pnclient);
}
void                        Server::reciveNewData(int fd)
{
    std::vector<std::string> cmd;
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    Client *client = getClient(fd);
    ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0);
    if (bytes <= 0)
    {
        log(RED, "Client is disconnected!");
        removeChannels(fd);
        removeClient(fd);
        removeFds(fd);
        close(fd);
    }
    else
    {
        client->setBuffer(buff);
        if (client->getBuffer().find_first_of("\r\n") == std::string::npos)
            return ;
        cmd = split_recivedBuffer(client->getBuffer());
        for(size_t i = 0; i < cmd.size(); i++)
			this->parse_exec_cmd(cmd[i], fd);
        if(getClient(fd))
			getClient(fd)->clear_buffer();
    }
}
//Parsing methods
std::vector<std::string>    Server::split_recivedBuffer(std::string str)
{
    std::vector<std::string> vec;
	std::istringstream stm(str);
	std::string line;
	while(std::getline(stm, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		vec.push_back(line);
	}
	return vec;
}
std::vector<std::string>    Server::split_cmd(std::string &cmd)
{
    std::vector<std::string> vec;
	std::istringstream stm(cmd);
	std::string token;
	while(stm >> token)
	{
		vec.push_back(token);
		token.clear();
	}
	return vec;
}
void                        Server::parse_exec_cmd(std::string &cmd, int fd)
{
	if(cmd.empty())
		return ;
	std::vector<std::string> splited_cmd = split_cmd(cmd);
	size_t found = cmd.find_first_not_of(" \t\v");
	if(found != std::string::npos)
		cmd = cmd.substr(found);
	if(splited_cmd.size() && (splited_cmd[0] == "BONG" || splited_cmd[0] == "bong"))
		return;
    if(splited_cmd.size() && (splited_cmd[0] == "PASS" || splited_cmd[0] == "pass"))
        client_authen(fd, cmd);
	else if (splited_cmd.size() && (splited_cmd[0] == "NICK" || splited_cmd[0] == "nick"))
		setNickname(cmd,fd);
	else if(splited_cmd.size() && (splited_cmd[0] == "USER" || splited_cmd[0] == "user"))
		setUsername(cmd, fd);
	else if (splited_cmd.size() && (splited_cmd[0] == "QUIT" || splited_cmd[0] == "quit"))
		QUIT(cmd,fd);
	else if(notregistered(fd))
	{
		if (splited_cmd.size() && (splited_cmd[0] == "KICK" || splited_cmd[0] == "kick"))
			KICK(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "JOIN" || splited_cmd[0] == "join"))
			JOIN(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "TOPIC" || splited_cmd[0] == "topic"))
			Topic(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "MODE" || splited_cmd[0] == "mode"))
			mode_command(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "PART" || splited_cmd[0] == "part"))
			PART(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "PRIVMSG" || splited_cmd[0] == "privmsg"))
			PRIVMSG(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "INVITE" || splited_cmd[0] == "invite"))
			Invite(cmd,fd);
		else if (splited_cmd.size())
			_sendResponse(ERR_CMDNOTFOUND(getClient(fd)->getNickname(),splited_cmd[0]),fd);
	}
	else if (!notregistered(fd))
		_sendResponse(RPL_REGISTERED(std::string("*")),fd);
}