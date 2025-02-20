#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Server.hpp"

class Client;
class Channel
{
private:

	int 								invit_only;
	int 								topic;
	int 								key;
	int 								limit;
	bool 								topic_restriction;
	std::string 						name;
	std::string 						time_creation;
	std::string 						password;
	std::string 						created_at;
	std::string 						topic_name;
	std::vector<Client> 				clients;
	std::vector<Client> 				admins;
	std::vector<std::pair<char, bool> > modes;
public:
	Channel();
	~Channel();
	Channel(Channel const &src);
	Channel 							&operator=(Channel const &src);
	//---------------//Setters
	void 								setInvitOnly(int invit_only);
	void 								setTopic(int topic);
	void 								setKey(int key);
	void 								setLimit(int limit);
	void 								setTopicName(std::string topic_name);
	void 								setPassword(std::string password);
	void 								setName(std::string name);
	void 								setTime(std::string time);
	void 								set_topicRestriction(bool value);
	void 								setModeAtindex(size_t index, bool mode);
	void 								set_createiontime();
	//---------------//Getters
	int 								getInvitOnly();
	int 								getTopic();
	int 								getKey();
	int 								getLimit();
	int 								getClientsNumber();
	bool 								gettopic_restriction() const;
	bool 								getModeAtindex(size_t index);
	bool 								clientInChannel(std::string &nick);
	std::string 						getTopicName();
	std::string 						getPassword();
	std::string 						getName();
	std::string 						getTime();
	std::string 						get_creationtime();
	std::string 						getModes();
	std::string 						clientChannel_list();
	Client 								*get_client(int fd);
	Client 								*get_admin(int fd);
	Client 								*GetClientInChannel(std::string name);
	//---------------//Methods
	void 								add_client(Client newClient);
	void 								add_admin(Client newClient);
	void 								remove_client(int fd);
	void 								remove_admin(int fd);
	bool 								change_clientToAdmin(std::string& nick);
	bool 								change_adminToClient(std::string& nick);
	//---------------//SendToAll
	void 								sendTo_all(std::string rpl1);
	void 								sendTo_all(std::string rpl1, int fd);
};

#endif
