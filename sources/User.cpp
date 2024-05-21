#include "../headers/mainHeader.hpp"

void User::setuserFD(int userFD) {
	_userFD = userFD;
}

void User::setUserName(std::string userName) {
	_userName = userName;
}

void User::setRealName(std::string realName) {
	_realName = realName;
}

void User::setNickName(std::string nickName) {
	_nickName = nickName;
}

void User::setuserIP(char *userIP) {
	_userIP = userIP;
}

int User::getuserFD() {
	return _userFD;
}

std::string User::getRealName() {
	return _realName;
}

std::string User::getNickName() {
	return _nickName;
}

std::string User::getUserName() {
	return _userName;
}

std::string User::getuserIP() {
	return _userIP;
}

User::User(const int userFD) : _userFD(userFD), _nickName(""), _realName(""), _userName(""), _password("") {
}

std::string User::getPassword() {
	return _password;
}

void User::setPassword(std::string password) {
	_password = password;
}


bool User::isAuth() {
	if(this->_userName.empty() || this->_nickName.empty() || this->_password.empty())
		return false;
	return true;
}
