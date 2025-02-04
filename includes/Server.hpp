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
#include <ctime>
#include "Client.hpp"
#include "Channel.hpp"

class Cilent;
class Channel;

class Server
{
    private:
        int                         _port;
        int                         _socket;
        static bool                 _signal_f; // for signal handle flag setting
        std::string                 _pass;
        std::vector<Client>         _clients;
        std::vector<Channel>        _channels;
        std::vector<struct pollfd>  _pfds;
        struct sockaddr_in          _add;
        struct sockaddr_in          _cadd; // client add
        struct pollfd               _pnclient; // poll new client
    public:
        /* Constructors */
        Server();
        Server(const Server &src);
        Server                      &operator=(const Server &src);
        ~Server();
        /* Getters */
        int                         getFd() const;
        int                         getPort() const;
        std::string                 getPass() const;
        Client*                     getClient(int fd) const;
        Client*                     getClientNick(std::string nickname);
        Channel*                    getChannel(std::string name);
        /* Setters */
        void                        setFd(int socket);
        void                        setPort(int port);
        void                        setPassword(std::string pass);
        void                        setUsername(std::string &username, int fd); // Auth class method
        void                        setNickname(std::string cmd, int fd);       // Auth class method
        /* Additional methods */
        void                        addClient(Client nclient);
        void                        addChannel(Channel nchannel);
        void                        addFds(pollfd npfd);
        /* Remove&Clear methods */
        void                        removeClient(int fd);
        void                        removeChannel(int fd);
        void                        removeFds(int fd);
        void                        removeChannels(int fd);
        /* Send methods */
        void                        sendError(int code, std::string clientname, int fd, std::string msg);
        void                        sendError(int code, std::string clientname, std::string channelname, int fd, std::string msg); //TODO: create one log function that can handle all 3 funcitons
        void                        _sendResponse(std::string response, int fd);
        /* Signal Handlers */
        static void                 signalHandler(int signum);
        void                        closeFds();
        /* MAIN Server methods */
        void                        init(int port, std::string pass);
        void                        acceptClient();
        void                        socket_create();
        void                        reciveNewData(int fd);
        /* Parsing methods */
        std::vector<std::string>    split_recivedBuffer(std::string str);
        std::vector<std::string>    split_cmd(std::string &cmd);
        void                        parse_exec_cmd(std::string &cmd, int fd);
        /* Authentification Methods */
        bool                        notregistered(int fd);
        bool                        nickNameInUse(std::string& nickname);
        bool                        is_validNickname(std::string& nickname);
        void                        client_authen(int fd, std::string pass);
        /* JOIN CMD */
        void	                    JOIN(std::string cmd, int fd);
        int		                    SplitJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd);
        void	                    ExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd);
        void	                    NotExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int fd);
        /* PART CMD */
        void	                    PART(std::string cmd, int fd);
        int		                    SplitCmdPart(std::string cmd, std::vector<std::string> &tmp, std::string &reason, int fd);
        /* KICK CMD */
        void	                    KICK(std::string cmd, int fd);
        std::string                 SplitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd);
        /* PRIVMSG CMD */
        void	                    PRIVMSG(std::string cmd, int fd);
        void	                    CheckForChannels_Clients(std::vector<std::string> &tmp, int fd);
        /* QUITE CMD */
        void                        QUIT(std::string cmd, int fd);
        /* MODE CMD */
        void 		                mode_command(std::string& cmd, int fd);
        std::string                 invite_only(Channel *channel, char opera, std::string chain);
        std::string                 topic_restriction(Channel *channel ,char opera, std::string chain);
        std::string                 password_mode(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments);
        std::string                 operator_privilege(std::vector<std::string> splited, Channel *channel, size_t& pos, int fd, char opera, std::string chain, std::string& arguments);
        std::string                 channel_limit(std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments);
        bool		                isvalid_limit(std::string& limit);
        std::string                 mode_toAppend(std::string chain, char opera, char mode);
        std::vector<std::string>    splitParams(std::string params);
        void                        getCmdArgs(std::string cmd,std::string& name, std::string& modeset ,std::string &params);
        /* TOPIC CMD */
 	    std::string                 tTopic();
	    void                        Topic(std::string &cmd, int &fd);
	    void                        Invite(std::string &cmd, int &fd);
	    std::string                 getTopic(std::string& input);
	    int                         getPos(std::string &cmd);       

};      
