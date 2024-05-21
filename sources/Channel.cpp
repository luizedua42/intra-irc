#include "../headers/Channel.hpp"

void initModes(std::map<std::string, bool>& modes) {
	modes["o"] = false;
	modes["t"] = true;
	modes["i"] = false;
	modes["k"] = false;
	modes["l"] = false;
}

void Channel::promoteToOperator(const std::string userNickname) {
    std::map<std::string, User*>::iterator it = _users.find(userNickname);
    if (it != _users.end()) {
        _operators[userNickname] = it->second;
        _users.erase(it);
    }
}

void Channel::demoteFromOperator(const std::string userNickname) {
    std::map<std::string, User*>::iterator it = _operators.find(userNickname);
    if (it != _operators.end()) {
        _users[userNickname] = it->second;
        _operators.erase(it);
    }
}

std::map<std::string, User*> Channel::getOperators() const {
    return this->_operators;
}

std::map<std::string, User*> Channel::getNonOperators() const {
    return this->_users;
}

std::vector<User*> Channel::getAllUsers() const {
	std::vector<User*> allUsers;
	
	for (std::map<std::string, User*>::const_iterator it = _operators.begin(); it != _operators.end(); it++) {
		allUsers.push_back(it->second);
	}
	for (std::map<std::string, User*>::const_iterator it = _users.begin(); it != _users.end(); it++) {
		allUsers.push_back(it->second);
	}
	
	return allUsers;
}

std::string Channel::getName() const {
	return this->_name;
}

bool Channel::getMode(std::string mode) const {
	return this->_modes.at(mode);
}

std::string Channel::getAllModes() const {
	std::string modes = "+";
	if (_modes.at("o")) modes += "o";
	if (_modes.at("t")) modes += "t";
	if (_modes.at("i")) modes += "i";
	if (_modes.at("k")) modes += "k";
	if (_modes.at("l")) modes += "l";
	return modes;
}

std::string Channel::getPassword() const {
	return this->_password;
}

void Channel::setName(const std::string& name) {
	this->_name = name;
}

void Channel::addUser(User* User) {
	_users.insert(std::make_pair(User->getNickName(), User));
	incrementUserCount();
}

void Channel::addToInviteList(const std::string nickname) {
    _inviteList.push_back(nickname);
}

void Channel::removeFromInviteList(const std::string nickname) {
    for (std::vector<std::string>::iterator it = _inviteList.begin(); it != _inviteList.end(); ++it) {
        if (*it == nickname) {
            _inviteList.erase(it);
            break;
        }
    }
}

bool Channel::removeUser(const std::string& nickname) {
    std::map<std::string, User*>::iterator it = _users.find(nickname);

    if (it != _users.end()) {
        _users.erase(it);
		return true;
    } else {
		std::string response = IRC + ERR_NOTONCHANNELNBR + _name + ERR_NOTONCHANNEL + END;
		return false;
    }
}

Channel::Channel(const std::string& name) : _name(name) {
	_topic = "";
	initModes(_modes);
	_password = "";
	_userCount = 0;
	_userLimit = MAX_USERS;
}

Channel::~Channel(void) {
	_users.clear();
	_operators.clear();
}

// Function to list all users' nicknames
void Channel::listUsers() const {
	std::cout << "List of Users:\n";
	for (std::map<std::string, User*>::const_iterator it = _users.begin(); it != _users.end(); it++) {
		std::cout << it->second->getNickName() << std::endl;
	}
}

// Function to list all operators' nicknames
void Channel::listOperators() const {
	std::cout << "List of Operators:\n";
	for (std::map<std::string, User*>::const_iterator it = _operators.begin(); it != _operators.end(); it++) {
		std::cout << it->second->getNickName() << std::endl;
	}
}

void Channel::setMode(std::string mode, bool value) {
	_modes[mode] = value;
}

void Channel::setPassword(std::string& password) {
	_password = password;
}

void Channel::setTopic(const std::string& topic) {
	_topic = topic;
}

std::string Channel::getTopic() const {
	return this->_topic;
}

void Channel::setUserLimit(int limit) {
	_userLimit = limit;
}

int Channel::getUserLimit() const {
	return _userLimit;
}

bool Channel::isUserOperator(const std::string userNickname) const {
	return _operators.find(userNickname) != _operators.end();
}

bool Channel::isUserInvited(const std::string userNickname) const {
	for (std::vector<std::string>::const_iterator it = _inviteList.begin(); it != _inviteList.end(); ++it) {
		if (*it == userNickname) {
			return true;
		}
	}
	return false;
}

bool Channel::isUserOnChannel(const std::string userNickname) const {
    bool isUserOnChannel = (_users.find(userNickname) != _users.end());
    bool isUserOperator = (_operators.find(userNickname) != _operators.end());

    return isUserOnChannel || isUserOperator;
}

int Channel::getUserCount() const {
	return _userCount;
}

void Channel::decrementUserCount(void) {
	_userCount--;
}

void Channel::incrementUserCount(void) {
	_userCount++;
}

std::string Channel::getChannelUsersList() const {
	std::string usersList;
	for (std::map<std::string, User*>::const_iterator it = _users.begin(); it != _users.end(); it++) {
		usersList += it->first + " ";
	}
	for (std::map<std::string, User*>::const_iterator it = _operators.begin(); it != _operators.end(); it++) {
		usersList += '@';
		usersList += it->first + " ";
	}
	return usersList;
}