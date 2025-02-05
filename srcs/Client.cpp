#include "Client.hpp"
//Constructors
Client::Client() 
{
    this->_nickname = "";
	this->_username = "";
	this->_fd = -1;
	this->_isOperator= false;
	this->_registered = false;
	this->_buffer = "";
	this->_idaddress = "";
	this->_logedin = false;
}
Client::Client(std::string nickname, std::string username, int fd)
    : _fd(fd), _nickname(nickname), _username(username) {}
Client::~Client(){}
Client::Client(const Client &src){*this = src;}
Client      &Client::operator=(const Client &src)
{(void)src; return *this;}
// Gettors
int         Client::getFd(){return this->_fd;}
bool        Client::getRegistered() const{return this->_registered;}
bool        Client::getInviteChannel(std::string &chaname) const
{
    for (size_t i = 0; i < this->ChannelsInvite.size(); i++)
    {
        if (this->ChannelsInvite[i] == chaname)
            return (true);        
    }
    return (false);
}
std::string Client::getNickname() const{return this->_nickname;}
bool        Client::getLogedin() const{return this->_logedin;}
std::string Client::getUserName() const{return this->_username;}
std::string Client::getIpAdd() {return _idaddress;}
std::string Client::getBuffer() { return this->_buffer; }
std::string Client::getHostname() const
{
    std::string hostname = this->getNickname() + "!" + this->getUserName();
    return hostname;
}
// Settors
void        Client::setFd(int fd){this->_fd = fd;}
void        Client::setNickname(std::string& nickname){this->_nickname = nickname;}
void        Client::setLogedin(bool value){this->_logedin = value;}
void        Client::setUsername(std::string& username){this->_username = username;}
void        Client::setBuffer(std::string recived){_buffer += recived;}
void        Client::setRegistered(bool value){this->_registered = value;}
void        Client::setIpAdd(std::string ipadd){this->_idaddress = ipadd;}
// Methods
void        Client::clear_buffer(){_buffer.clear();}
void        Client::addchannelInvite(std::string &channelname){ChannelsInvite.push_back(channelname);}
void        Client::rmchannelInvite(std::string &channelname)
{
	for (size_t i = 0; i < this->ChannelsInvite.size(); i++){
		if (this->ChannelsInvite[i] == channelname)
			{this->ChannelsInvite.erase(this->ChannelsInvite.begin() + i); return;}
	}
}