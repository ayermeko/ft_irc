#include "Channel.hpp"

Channel::Channel(){
	this->invit_only = 0;
	this->topic = 0;
	this->key = 0;
	this->limit = 0;
	this->topic_restriction = false;
	this->name = "";
	this->topic_name = "";
	char charaters[5] = {'i', 't', 'k', 'o', 'l'};
	for(int i = 0; i < 5; i++)
		modes.push_back(std::make_pair(charaters[i],false));
	this->created_at = "";
}
Channel::~Channel(){}
Channel::Channel(Channel const &src){*this = src;}
Channel &Channel::operator=(Channel const &src){
	if (this != &src){
		this->invit_only = src.invit_only;
		this->topic = src.topic;
		this->key = src.key;
		this->limit = src.limit;
		this->topic_restriction = src.topic_restriction;
		this->name = src.name;
		this->password = src.password;
		this->created_at = src.created_at;
		this->topic_name = src.topic_name;
		this->clients = src.clients;
		this->admins = src.admins;
		this->modes = src.modes;
	}
	return *this;
}
//---------------//Setters
void Channel::setInvitOnly(int invit_only){this->invit_only = invit_only;}
void Channel::setTopic(int topic){this->topic = topic;}
void Channel::setTime(std::string time){this->time_creation = time;}
void Channel::setKey(int key){this->key = key;}
void Channel::setLimit(int limit){this->limit = limit;}
void Channel::setTopicName(std::string topic_name){this->topic_name = topic_name;}
void Channel::setPassword(std::string password){this->password = password;}
void Channel::setName(std::string name){this->name = name;}
void Channel::set_topicRestriction(bool value){this->topic_restriction = value;}
void Channel::setModeAtindex(size_t index, bool mode){modes[index].second = mode;}
void Channel::set_createiontime(){
	std::time_t _time = std::time(NULL);
	std::ostringstream oss;
	oss << _time;
	this->created_at = std::string(oss.str());
}
//---------------//Setters
//---------------//Getters
int Channel::getInvitOnly(){return this->invit_only;}
int Channel::getTopic(){return this->topic;}
int Channel::getKey(){return this->key;}
int Channel::getLimit(){return this->limit;}
int Channel::getClientsNumber(){return this->clients.size() + this->admins.size();}
bool Channel::gettopic_restriction() const{return this->topic_restriction;}
bool Channel::getModeAtindex(size_t index){return modes[index].second;}
bool Channel::clientInChannel(std::string &nick){
	for(size_t i = 0; i < clients.size(); i++){
		if(clients[i].getNickName() == nick)
			return true;
	}
	for(size_t i = 0; i < admins.size(); i++){
		if(admins[i].getNickName() == nick)
			return true;
	}
	return false;
}
std::string Channel::getTopicName(){return this->topic_name;}
std::string Channel::getPassword(){return this->password;}
std::string Channel::getName(){return this->name;}
std::string Channel::getTime(){return this->time_creation;}
std::string Channel::get_creationtime(){return created_at;}
std::string Channel::getModes(){
	std::string mode;
	for(size_t i = 0; i < modes.size(); i++){
		if(modes[i].first != 'o' && modes[i].second)
			mode.push_back(modes[i].first);
	}
	if(!mode.empty())
		mode.insert(mode.begin(),'+');
	return mode;
}
std::string Channel::clientChannel_list(){
	std::string list;
	for(size_t i = 0; i < admins.size(); i++){
		list += "@" + admins[i].getNickName();
		if((i + 1) < admins.size())
			list += " ";
	}
	if(clients.size())
		list += " ";
	for(size_t i = 0; i < clients.size(); i++){
		list += clients[i].getNickName();
		if((i + 1) < clients.size())
			list += " ";
	}
	return list;
}
Client *Channel::get_client(int fd){
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it){
		if (it->getFd() == fd)
			return &(*it);
	}
	return NULL;
}
Client *Channel::get_admin(int fd){
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it){
		if (it->getFd() == fd)
			return &(*it);
	}
	return NULL;
}
Client* Channel::GetClientInChannel(std::string name)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it){
		if (it->getNickName() == name)
			return &(*it);
	}
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it){
		if (it->getNickName() == name)
			return &(*it);
	}
	return NULL;
}
//---------------//Getters
//---------------//Methods
void Channel::add_client(Client newClient){clients.push_back(newClient);}
void Channel::add_admin(Client newClient){admins.push_back(newClient);}
void Channel::remove_client(int fd){
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it){
		if (it->getFd() == fd)
			{clients.erase(it); break;}
	}
}
void Channel::remove_admin(int fd){
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it){
		if (it->getFd() == fd)
			{admins.erase(it); break;}
	}
}
bool Channel::change_clientToAdmin(std::string& nick){
	size_t i = 0;
	for(; i < clients.size(); i++){
		if(clients[i].getNickName() == nick)
			break;
	}
	if(i < clients.size()){
		admins.push_back(clients[i]);
		clients.erase(i + clients.begin());
		return true;
	}
	return false;
}

bool Channel::change_adminToClient(std::string& nick){
	size_t i = 0;
	for(; i < admins.size(); i++){
		if(admins[i].getNickName() == nick)
			break;
	}
	if(i < admins.size()){
		clients.push_back(admins[i]);
		admins.erase(i + admins.begin());
		return true;
	}
	return false;

}
//---------------//Methods
//---------------//SendToAll
void Channel::sendTo_all(std::string rpl1)
{
	for(size_t i = 0; i < admins.size(); i++)
		if(send(admins[i].getFd(), rpl1.c_str(), rpl1.size(),0) == -1)
			std::cerr << "send() faild" << std::endl;
	for(size_t i = 0; i < clients.size(); i++)
		if(send(clients[i].getFd(), rpl1.c_str(), rpl1.size(),0) == -1)
			std::cerr << "send() faild" << std::endl;
}
void Channel::sendTo_all(std::string rpl1, int fd)
{
	for(size_t i = 0; i < admins.size(); i++){
		if(admins[i].getFd() != fd)
			if(send(admins[i].getFd(), rpl1.c_str(), rpl1.size(),0) == -1)
				std::cerr << "send() faild" << std::endl;
	}
	for(size_t i = 0; i < clients.size(); i++){
		if(clients[i].getFd() != fd)
			if(send(clients[i].getFd(), rpl1.c_str(), rpl1.size(),0) == -1)
				std::cerr << "send() faild" << std::endl;
	}
}
//---------------//SendToAll