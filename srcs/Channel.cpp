#include "Channel.hpp"

Channel::Channel(){
	this->_invit_only = 0;
	this->_topic = 0;
	this->_key = 0;
	this->_limit = 0;
	this->_topic_restriction = false;
	this->_name = "";
	this->_topic_name = "";
	char charaters[5] = {'i', 't', 'k', 'o', 'l'};
	for(int i = 0; i < 5; i++)
		_modes.push_back(std::make_pair(charaters[i],false));
	this->_created_at = "";
}
Channel::~Channel(){}
Channel::Channel(Channel const &src){*this = src;}
Channel &Channel::operator=(Channel const &src){(void)src; return *this;}
//---------------//Setters
void Channel::setInvitOnly(int invit_only){this->_invit_only = invit_only;}
void Channel::setTopic(int topic){this->_topic = topic;}
void Channel::setTime(std::string time){this->_time_creation = time;}
void Channel::setKey(int key){this->_key = key;}
void Channel::setLimit(int limit){this->_limit = limit;}
void Channel::setTopicName(std::string topic_name){this->_topic_name = topic_name;}
void Channel::setPassword(std::string password){this->_password = password;}
void Channel::setName(std::string name){this->_name = name;}
void Channel::set_topicRestriction(bool value){this->_topic_restriction = value;}
void Channel::setModeAtindex(size_t index, bool mode){_modes[index].second = mode;}
void Channel::set_createiontime(){
	std::time_t _time = std::time(NULL);
	std::ostringstream oss;
	oss << _time;
	this->_created_at = std::string(oss.str());
}
//---------------//Setters
//---------------//Getters
int Channel::getInvitOnly(){return this->_invit_only;}
int Channel::getTopic(){return this->_topic;}
int Channel::getKey(){return this->_key;}
int Channel::getLimit(){return this->_limit;}
int Channel::getClientsNumber(){return this->_clients.size() + this->_admins.size();}
bool Channel::gettopic_restriction() const{return this->_topic_restriction;}
bool Channel::getModeAtindex(size_t index){return _modes[index].second;}
bool Channel::clientInChannel(std::string &nick){
	for(size_t i = 0; i < _clients.size(); i++){
		if(_clients[i].getNickName() == nick)
			return true;
	}
	for(size_t i = 0; i < _admins.size(); i++){
		if(_admins[i].getNickName() == nick)
			return true;
	}
	return false;
}
std::string Channel::getTopicName(){return this->_topic_name;}
std::string Channel::getPassword(){return this->_password;}
std::string Channel::getName(){return this->_name;}
std::string Channel::getTime(){return this->_time_creation;}
std::string Channel::get_creationtime(){return _created_at;}
std::string Channel::getModes(){
	std::string mode;
	for(size_t i = 0; i < _modes.size(); i++){
		if(_modes[i].first != 'o' && _modes[i].second)
			mode.push_back(_modes[i].first);
	}
	if(!mode.empty())
		mode.insert(mode.begin(),'+');
	return mode;
}
std::string Channel::clientChannel_list(){
	std::string list;
	for(size_t i = 0; i < _admins.size(); i++){
		list += "@" + _admins[i].getNickName();
		if((i + 1) < _admins.size())
			list += " ";
	}
	if(_clients.size())
		list += " ";
	for(size_t i = 0; i < _clients.size(); i++){
		list += _clients[i].getNickName();
		if((i + 1) < _clients.size())
			list += " ";
	}
	return list;
}
Client *Channel::get_client(int fd){
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it){
		if (it->getFd() == fd)
			return &(*it);
	}
	return NULL;
}
Client *Channel::get_admin(int fd){
	for (std::vector<Client>::iterator it = _admins.begin(); it != _admins.end(); ++it){
		if (it->getFd() == fd)
			return &(*it);
	}
	return NULL;
}
Client* Channel::GetClientInChannel(std::string name)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it){
		if (it->getNickName() == name)
			return &(*it);
	}
	for (std::vector<Client>::iterator it = _admins.begin(); it != _admins.end(); ++it){
		if (it->getNickName() == name)
			return &(*it);
	}
	return NULL;
}
//---------------//Getters
//---------------//Methods
void Channel::add_client(Client newClient){_clients.push_back(newClient);}
void Channel::add_admin(Client newClient){_admins.push_back(newClient);}
void Channel::remove_client(int fd){
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it){
		if (it->getFd() == fd)
			{_clients.erase(it); break;}
	}
}
void Channel::remove_admin(int fd){
	for (std::vector<Client>::iterator it = _admins.begin(); it != _admins.end(); ++it){
		if (it->getFd() == fd)
			{_admins.erase(it); break;}
	}
}
bool Channel::change_clientToAdmin(std::string& nick){
	size_t i = 0;
	for(; i < _clients.size(); i++){
		if(_clients[i].getNickName() == nick)
			break;
	}
	if(i < _clients.size()){
		_admins.push_back(_clients[i]);
		_clients.erase(i + _clients.begin());
		return true;
	}
	return false;
}

bool Channel::change_adminToClient(std::string& nick){
	size_t i = 0;
	for(; i < _admins.size(); i++){
		if(_admins[i].getNickName() == nick)
			break;
	}
	if(i < _admins.size()){
		_clients.push_back(_admins[i]);
		_admins.erase(i + _admins.begin());
		return true;
	}
	return false;

}
//---------------//Methods
//---------------//SendToAll
void Channel::sendTo_all(std::string rpl1)
{
	for(size_t i = 0; i < _admins.size(); i++)
		if(send(_admins[i].getFd(), rpl1.c_str(), rpl1.size(),0) == -1)
			std::cerr << "send() faild" << std::endl;
	for(size_t i = 0; i < _clients.size(); i++)
		if(send(_clients[i].getFd(), rpl1.c_str(), rpl1.size(),0) == -1)
			std::cerr << "send() faild" << std::endl;
}
void Channel::sendTo_all(std::string rpl1, int fd)
{
	for(size_t i = 0; i < _admins.size(); i++){
		if(_admins[i].getFd() != fd)
			if(send(_admins[i].getFd(), rpl1.c_str(), rpl1.size(),0) == -1)
				std::cerr << "send() faild" << std::endl;
	}
	for(size_t i = 0; i < _clients.size(); i++){
		if(_clients[i].getFd() != fd)
			if(send(_clients[i].getFd(), rpl1.c_str(), rpl1.size(),0) == -1)
				std::cerr << "send() faild" << std::endl;
	}
}
//---------------//SendToAll