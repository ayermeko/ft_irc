#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Channel.hpp"

class Client
{
private:
	int 						_fd;
	bool 						_isOperator;
	bool 						_registered;
	std::string 				_nickname;
	bool 						_logedin;
	std::string 				_username;	
	std::string 				_buffer;
	std::string 				_ipadd;
	std::vector<std::string> 	ChannelsInvite;
public:
	Client();
	Client(std::string nickname, std::string username, int fd);
	~Client();
	Client(Client const &src);
	Client 						&operator=(Client const &src);
	//---------------//Getters
	int 						getFd();
	bool 						getRegistered();
	bool 						getInviteChannel(std::string &ChName);
	std::string 				getNickName();
	bool 						getLogedIn();
	std::string 				getUserName();
	std::string 				getIpAdd();
	std::string 				getBuffer();
	std::string 				getHostname();
	//---------------//Setters
	void 						setFd(int fd);
	void 						setNickname(std::string& nickName);
	void 						setLogedin(bool value);
	void 						setUsername(std::string& username);
	void 						setBuffer(std::string recived);
	void 						setRegistered(bool value);
	void 						setIpAdd(std::string ipadd);
	//---------------//Methods
	void 						clearBuffer();
	void 						AddChannelInvite(std::string &chname);
	void 						RmChannelInvite(std::string &chname);
};

#endif