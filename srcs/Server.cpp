#include "Server.hpp"

Server::Server(){this->_server_fdsocket = -1;}
Server::~Server(){}
Server::Server(Server const &src){*this = src;}
Server &Server::operator=(Server const &src){
	(void)src;
	return *this;
}
//---------------//Getters
int Server::getPort(){return this->_port;}
int Server::getFd(){return this->_server_fdsocket;}
Client *Server::getClient(int fd){
	for (size_t i = 0; i < this->_clients.size(); i++){
		if (this->_clients[i].getFd() == fd)
			return &this->_clients[i];
	}
	return NULL;
}
Client *Server::getClientNick(std::string nickname){
	for (size_t i = 0; i < this->_clients.size(); i++){
		if (this->_clients[i].getNickName() == nickname)
			return &this->_clients[i];
	}
	return NULL;
}

Channel *Server::getChannel(std::string name)
{
	for (size_t i = 0; i < this->_channels.size(); i++){
		if (this->_channels[i].getName() == name)
			return &_channels[i];
	}
	return NULL;
}
//---------------//Getters
//---------------//Setters
void Server::setFd(int fd){this->_server_fdsocket = fd;}
void Server::setPort(int port){this->_port = port;}
void Server::setPassword(std::string password){this->_password = password;}
std::string Server::getPassword(){return this->_password;}
void Server::addClient(Client newClient){this->_clients.push_back(newClient);}
void Server::addChannel(Channel newChannel){this->_channels.push_back(newChannel);}
void Server::addFds(pollfd newFd){this->_fds.push_back(newFd);}
//---------------//Setters
//---------------//Remove Methods
void Server::removeClient(int fd){
	for (size_t i = 0; i < this->_clients.size(); i++){
		if (this->_clients[i].getFd() == fd)
			{this->_clients.erase(this->_clients.begin() + i); return;}
	}
}
void Server::removeChannel(std::string name){
	for (size_t i = 0; i < this->_channels.size(); i++){
		if (this->_channels[i].getName() == name)
			{this->_channels.erase(this->_channels.begin() + i); return;}
	}
}
void Server::removeFds(int fd){
	for (size_t i = 0; i < this->_fds.size(); i++){
		if (this->_fds[i].fd == fd)
			{this->_fds.erase(this->_fds.begin() + i); return;}
	}
}
void	Server::rmChannels(int fd){
	for (size_t i = 0; i < this->_channels.size(); i++){
		int flag = 0;
		if (_channels[i].get_client(fd))
			{_channels[i].remove_client(fd); flag = 1;}
		else if (_channels[i].get_admin(fd))
			{_channels[i].remove_admin(fd); flag = 1;}
		if (_channels[i].getClientsNumber() == 0)
			{_channels.erase(_channels.begin() + i); i--; continue;}
		if (flag){
			std::string rpl = ":" + getClient(fd)->getNickName() + "!~" + getClient(fd)->getUserName() + "@localhost QUIT Quit\r\n";
			_channels[i].sendTo_all(rpl);
		}
	}
}
//---------------//Remove Methods
//---------------//Send Methods
void Server::senderror(int code, std::string clientname, int fd, std::string msg)
{
	std::stringstream ss;
	ss << ":localhost " << code << " " << clientname << msg;
	std::string resp = ss.str();
	if(send(fd, resp.c_str(), resp.size(),0) == -1)
		std::cerr << "send() faild" << std::endl;
}

void Server::senderror(int code, std::string clientname, std::string channelname, int fd, std::string msg)
{
	std::stringstream ss;
	ss << ":localhost " << code << " " << clientname << " " << channelname << msg;
	std::string resp = ss.str();
	if(send(fd, resp.c_str(), resp.size(),0) == -1)
		std::cerr << "send() faild" << std::endl;
}

void Server::_sendResponse(std::string response, int fd)
{
	if(send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() faild" << std::endl;
}
//---------------//Send Methods
//---------------//Close and Signal Methods
bool Server::_Signal = false;
void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::_Signal = true;
}

void	Server::close_fds(){
	for(size_t i = 0; i < _clients.size(); i++){
		log(YELLOW, "Client <" + my_itos(_clients[i].getFd()) + "> is disconnected!");
		close(_clients[i].getFd());
	}
	if (_server_fdsocket != -1){	
		log(YELLOW, "Server <" + my_itos(_server_fdsocket) + "> is disconnected!");
		close(_server_fdsocket);
	}
}
//---------------//Close and Signal Methods
//---------------//Server Methods
void Server::init(int port, std::string pass)
{
	this->_password = pass;
	this->_port = port;
	this->set_sever_socket();

	log(GREEN, "Server <" + my_itos(_server_fdsocket) + "> is created, waiting for connections...");
	while (Server::_Signal == false)
	{
		if((poll(&_fds[0],_fds.size(),-1) == -1) && Server::_Signal == false)
			throw(std::runtime_error("poll() faild"));
		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _server_fdsocket)
					this->accept_new_client();
				else
					this->reciveNewData(_fds[i].fd);
			}
		}
	}
	close_fds();
}

void Server::set_sever_socket()
{
	int en = 1;
	_add.sin_family = AF_INET;
	_add.sin_addr.s_addr = INADDR_ANY;
	_add.sin_port = htons(_port);
	_server_fdsocket = socket(AF_INET, SOCK_STREAM, 0);
	if(_server_fdsocket == -1)
		throw(std::runtime_error("faild to create socket"));
	if(setsockopt(_server_fdsocket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	 if (fcntl(_server_fdsocket, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(_server_fdsocket, (struct sockaddr *)&_add, sizeof(_add)) == -1)
		throw(std::runtime_error("faild to bind socket"));
	if (listen(_server_fdsocket, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() faild"));
	_new_cli.fd = _server_fdsocket;
	_new_cli.events = POLLIN;
	_new_cli.revents = 0;
	_fds.push_back(_new_cli);
}

void Server::accept_new_client()
{
	Client cli;
	memset(&_cliadd, 0, sizeof(_cliadd));
	socklen_t len = sizeof(_cliadd);
	int incofd = accept(_server_fdsocket, (sockaddr *)&(_cliadd), &len);
	if (incofd == -1)
		{std::cout << "accept() failed" << std::endl; return;}
	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1)
		{std::cout << "fcntl() failed" << std::endl; return;}
	_new_cli.fd = incofd;
	_new_cli.events = POLLIN;
	_new_cli.revents = 0;
	cli.setFd(incofd);
	cli.setIpAdd(inet_ntoa((_cliadd.sin_addr)));
	_clients.push_back(cli);
	_fds.push_back(_new_cli);
	log(GREEN, "Client <" + my_itos(cli.getFd()) + "> is connected to the server!");
}

void Server::reciveNewData(int fd)
{
	std::vector<std::string> cmd;
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	Client *cli = getClient(fd);
	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0);
	if(bytes <= 0)
	{
		log(YELLOW, "Client <" + my_itos(fd) + "> Disconnected!");
		rmChannels(fd);
		removeClient(fd);
		removeFds(fd);
		close(fd);
	}
	else
	{ 
		cli->setBuffer(buff);
		if(cli->getBuffer().find_first_of(CRLF) == std::string::npos)
			return;
		cmd = split_recivedBuffer(cli->getBuffer());
		for(size_t i = 0; i < cmd.size(); i++)
			this->parse_exec_cmd(cmd[i], fd);
		if(getClient(fd))
			getClient(fd)->clearBuffer();
	}
}
//---------------//Server Methods
//---------------//Parsing Methods
std::vector<std::string> Server::split_recivedBuffer(std::string str)
{
	std::vector<std::string> vec;
	std::istringstream stm(str);
	std::string line;
	while(std::getline(stm, line))
	{
		size_t pos = line.find_first_of(CRLF);
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		vec.push_back(line);
	}
	return vec;
}

std::vector<std::string> Server::split_cmd(std::string& cmd)
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

bool Server::notregistered(int fd)
{
	if (!getClient(fd) || getClient(fd)->getNickName().empty() || getClient(fd)->getUserName().empty() || getClient(fd)->getNickName() == "*"  || !getClient(fd)->getLogedIn())
		return false;
	return true;
}

void Server::parse_exec_cmd(std::string &cmd, int fd)
{
	if(cmd.empty())
		return ;
	std::vector<std::string> splited_cmd = split_cmd(cmd);
	size_t found = cmd.find_first_not_of(" \t\v");
	if(found != std::string::npos)
		cmd = cmd.substr(found);
	if(splited_cmd.size() && (splited_cmd[0] == "BONG" || splited_cmd[0] == "bong"))
		return ;
    if(splited_cmd.size() && (splited_cmd[0] == "PASS" || splited_cmd[0] == "pass"))
        client_authen(fd, cmd);
	else if (splited_cmd.size() && (splited_cmd[0] == "NICK" || splited_cmd[0] == "nick"))
		set_nickname(cmd,fd);
	else if(splited_cmd.size() && (splited_cmd[0] == "USER" || splited_cmd[0] == "user"))
		set_username(cmd, fd);
	else if (splited_cmd.size() && (splited_cmd[0] == "QUIT" || splited_cmd[0] == "quit"))
		QUIT(cmd,fd);
	else if(notregistered(fd))
	{
		if (splited_cmd.size() && (splited_cmd[0] == "KICK" || splited_cmd[0] == "kick"))
			KICK(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "JOIN" || splited_cmd[0] == "join"))
			JOIN(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "PRIVMSG" || splited_cmd[0] == "privmsg"))
			PRIVMSG(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "TOPIC" || splited_cmd[0] == "topic"))
			TOPIC(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "MODE" || splited_cmd[0] == "mode"))
			MODE(cmd, fd);
		else if (splited_cmd.size())
			_sendResponse(ERR_CMDNOTFOUND(getClient(fd)->getNickName(),splited_cmd[0]), fd);
	}
}
//---------------//Parsing Methods
//RFC 1459 IRC protocol format.