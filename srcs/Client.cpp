#include "Client.hpp"

Client::Client()
{
	this->_nickname = "";
	this->_username = "";
	this->_fd = -1;
	this->_isOperator= false;
	this->_registered = false;
	this->_buffer = "";
	this->_ipadd = "";
	this->_logedin = false;
}
Client::Client(std::string nickname, std::string username, int fd) : _fd(fd), _nickname(nickname), _username(username){}
Client::~Client(){}
Client::Client(Client const &src){*this = src;}
Client &Client::operator=(Client const &src){
	if (this != &src){
		this->_nickname = src._nickname;
		this->_username = src._username;
		this->_fd = src._fd;
		this->ChannelsInvite = src.ChannelsInvite;
		this->_buffer = src._buffer;
		this->_registered = src._registered;
		this->_ipadd = src._ipadd;
		this->_logedin = src._logedin;
	}
	return *this;
}
//---------------//Getters
int Client::getFd(){return this->_fd;}
bool Client::getRegistered(){return _registered;}
bool Client::getInviteChannel(std::string &ChName){
	for (size_t i = 0; i < this->ChannelsInvite.size(); i++){
		if (this->ChannelsInvite[i] == ChName)
			return true;
	}
	return false;
}
std::string Client::getNickName(){return this->_nickname;}
bool Client::getLogedIn(){return this->_logedin;}
std::string Client::getUserName(){return this->_username;}
std::string Client::getBuffer(){return _buffer;}
std::string Client::getIpAdd(){return _ipadd;}
std::string Client::getHostname(){
	std::string hostname = this->getNickName() + "!" + this->getUserName();
	return hostname;
}
//---------------//Getters
//---------------//Setters
void Client::setFd(int fd){this->_fd = fd;}
void Client::setNickname(std::string& nickName){this->_nickname = nickName;}
void Client::setLogedin(bool value){this->_logedin = value;}
void Client::setUsername(std::string& username){this->_username = username;}
void Client::setBuffer(std::string recived){_buffer += recived;}
void Client::setRegistered(bool value){_registered = value;}
void Client::setIpAdd(std::string ipadd){this->_ipadd = ipadd;}
//---------------//Setters
//---------------//Methods
void Client::clearBuffer(){_buffer.clear();}
void Client::AddChannelInvite(std::string &chname){
	ChannelsInvite.push_back(chname);
}
void Client::RmChannelInvite(std::string &chname){
	for (size_t i = 0; i < this->ChannelsInvite.size(); i++){
		if (this->ChannelsInvite[i] == chname)
			{this->ChannelsInvite.erase(this->ChannelsInvite.begin() + i); return;}
	}
}
//---------------//Methods