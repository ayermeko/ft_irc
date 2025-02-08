#pragma once

#include <iostream>
#include <vector>
#include <sstream>      // num to string conversion
#include <sys/socket.h> // socket();
#include <sys/types.h> 
#include <netinet/in.h> // sockaddr_in struct
#include <fcntl.h>      // fcntl() for socket options
#include <unistd.h>     // close() fd
#include <arpa/inet.h>  // in_addr struct, inet_ntoa()
#include <netdb.h>      // getnameinfo() hostname form an IP 
#include <poll.h>       // poll() for fd
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <fstream>      // file handle
#include "Client.hpp"
#include "Channel.hpp"
#include <ctime>
#include <string>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define CRLF "\r\n"

#define RPL_CONNECTED(nickname) (": 001 " + nickname + " : Welcome to the IRC server!" + CRLF)
#define RPL_REGISTERED(nickname) (": 002 " + nickname + " : You are now in the server!" + CRLF)
#define RPL_UMODEIS(hostname, channelname, mode, user)  ":" + hostname + " MODE " + channelname + " " + mode + " " + user + CRLF
#define RPL_CREATIONTIME(nickname, channelname, creationtime) ": 329 " + nickname + " #" + channelname + " " + creationtime + CRLF
#define RPL_CHANNELMODES(nickname, channelname, modes) ": 324 " + nickname + " #" + channelname + " " + modes + CRLF
#define RPL_CHANGEMODE(hostname, channelname, mode, arguments) (":" + hostname + " MODE #" + channelname + " " + mode + " " + arguments + CRLF)
#define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + CRLF)
#define RPL_JOINMSG(hostname, ipaddress, channelname) (":" + hostname + "@" + ipaddress + " JOIN #" + channelname + CRLF)
#define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " @ #" + channelname + " :" + clientslist + CRLF)
#define RPL_ENDOFNAMES(nickname, channelname) (": 366 " + nickname + " #" + channelname + " :END of /NAMES list" + CRLF)
#define RPL_TOPICIS(nickname, channelname, topic) (": 332 " + nickname + " #" +channelname + " :" + topic + "\r\n")

///////// ERRORS ////////////////
#define ERR_NEEDMODEPARM(channelname, mode) (": 696 #" + channelname + " * You must specify a parameter for the key mode. " + mode + CRLF)
#define ERR_INVALIDMODEPARM(channelname, mode) ": 696 #" + channelname + " Invalid mode parameter. " + mode + CRLF
#define ERR_KEYSET(channelname) ": 467 #" + channelname + " Channel key already set. " + CRLF
#define ERR_UNKNOWNMODE(nickname, channelname, mode) ": 472 " + nickname + " #" + channelname + " " + mode + " :is not a recognised channel mode" + CRLF
#define ERR_NOTENOUGHPARAM(nickname) (": 461 " + nickname + " :Not enough parameters." + CRLF)
#define ERR_CHANNELNOTFOUND(nickname, channelname) (": 403 " + nickname + " " + channelname + " :No such channel" + CRLF)
#define ERR_NOTOPERATOR(channelname) (": 482 #" + channelname + " :You're not a channel operator" + CRLF)
#define ERR_NOSUCHNICK(channelname, name) (": 401 #" + channelname + " " + name + " :No such nick/channel" + CRLF )
#define ERR_INCORPASS(nickname) (": 464 " + nickname + " :Password incorrect !" + CRLF )
#define ERR_ALREADYREGISTERED(nickname) (": 462 " + nickname + " :You may not reregister !" + CRLF )
#define ERR_NONICKNAME(nickname) (": 431 " + nickname + " :No nickname given" + CRLF )
#define ERR_NICKINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + CRLF)
#define ERR_ERRONEUSNICK(nickname) (": 432 " + nickname + " :Erroneus nickname" + CRLF)
#define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + CRLF)
#define ERR_CMDNOTFOUND(nickname, command) (": 421 " + nickname + " " + command + " :Unknown command" + CRLF)


class Client;
class Channel;

class Server
{
private:
	int 						_port;
	int 						_server_fdsocket;
	static bool 				_Signal;
	std::string 				_password;
	std::vector<Client> 		_clients;
	std::vector<Channel> 		_channels;
	std::vector<struct pollfd> 	_fds;
	struct sockaddr_in 			_add;
	struct sockaddr_in 			_cliadd;
	struct pollfd 				_new_cli;
public:
	Server();
	~Server();
	Server(Server const &src);
	Server 						&operator=(Server const &src);
	//---------------//Getters
	int 						getFd();
	int 						getPort();
	std::string 				getPassword();
	Client 					   *getClient(int fd);
	Client 					   *getClientNick(std::string nickname);
	Channel 				   *getChannel(std::string name);
	//---------------//Setters
	void 						setFd(int server_fdsocket);
	void 						setPort(int port);
	void 						setPassword(std::string password);
	void 						set_username(std::string& username, int fd);
	void 						set_nickname(std::string cmd, int fd);
	//---------------//Additional methods
	void 						addClient(Client newClient);
	void 						addChannel(Channel newChannel);
	void 						addFds(pollfd newFd);
	//---------------//Remove Methods
	void 						removeClient(int fd);
	void 						removeChannel(std::string name);
	void 						removeFds(int fd);
	void 						rmChannels(int fd);
	//---------------//Send Methods
	void 						senderror(int code, std::string clientname, int fd, std::string msg);
	void 						senderror(int code, std::string clientname, std::string channelname, int fd, std::string msg);
	void 						_sendResponse(std::string response, int fd);
	//---------------//Close and Signal Methods
	static void 				SignalHandler(int signum);
	void 						close_fds();
	//---------------//Server Methods
	void 						init(int port, std::string pass);
	void 						accept_new_client();
	void 						set_sever_socket();
	void 						reciveNewData(int fd);
	//---------------//Parsing Methods
	std::vector<std::string> 	split_recivedBuffer(std::string str);
	std::vector<std::string> 	split_cmd(std::string &str);
	void 						parse_exec_cmd(std::string &cmd, int fd);
	//---------------//Authentification Methods
	bool 						notregistered(int fd);
	bool 						nickNameInUse(std::string& nickname);
	bool 						is_validNickname(std::string& nickname);
	void 						client_authen(int fd, std::string pass);
	//---------------------------//JOIN CMD
	void						JOIN(std::string cmd, int fd);
	int							SplitJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd);
	void						ExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd);
	void						NotExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int fd);
	int							SearchForClients(std::string nickname);
	//---------------------------//PART CMD
	void						PART(std::string cmd, int fd);
	int							SplitCmdPart(std::string cmd, std::vector<std::string> &tmp, std::string &reason, int fd);
	//---------------------------//CKIK CMD
	void						KICK(std::string cmd, int fd);
	std::string 				SplitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd);
	//---------------------------//PRIVMSG CMD
	void						PRIVMSG(std::string cmd, int fd);
	void						CheckForChannels_Clients(std::vector<std::string> &tmp, int fd);
	//---------------------------//QUITE CMD
	void						QUIT(std::string cmd, int fd);
	//---------------------------//MODE CMD
	void 						mode_command(std::string& cmd, int fd);
	std::string 				invite_only(Channel *channel, char opera, std::string chain);
	std::string 				topic_restriction(Channel *channel ,char opera, std::string chain);
	std::string 				password_mode(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments);
	std::string 				operator_privilege(std::vector<std::string> splited, Channel *channel, size_t& pos, int fd, char opera, std::string chain, std::string& arguments);
	std::string 				channel_limit(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments);
	bool						isvalid_limit(std::string& limit);
	std::string 				mode_toAppend(std::string chain, char opera, char mode);
	std::vector<std::string> 	splitParams(std::string params);
	void 						getCmdArgs(std::string cmd,std::string& name, std::string& modeset ,std::string &params);
	//---------------------------//TOPIC CMD
	std::string 				tTopic();
	void 						Topic(std::string &cmd, int &fd);
	void 						Invite(std::string &cmd, int &fd);
	std::string 				gettopic(std::string& input);
	int 						getpos(std::string &cmd);
};

static inline void log(const std::string color, const std::string& message) {
    // Get the current time and format it
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    // Output the formatted log message with timestamp
    std::cout   << "\033[0;34m[" << buffer << "]\033[0m " 
                << color << message << "\033[0m " <<std::endl;
}

static inline std::string my_itos(int num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}
