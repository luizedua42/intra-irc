#include "../../headers/mainHeader.hpp"

void Server::nick(std::vector<std::string> options, int userFD) {
	std::string response;
	if(options.size() < 1){
		response = IRC + ERR_NONICKNAMEGIVENNBR + ERR_NONICKNAMEGIVEN + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	}
	User* user = Server::getUserByFD(userFD);
	if(user == NULL)
		return;

	std::string nickname = options[0].substr(0, options[0].find('\r'));
	if(nickname.empty()) {
		response = IRC + ERR_ERRONEUSNICKNAMENBR + nickname + ERR_ERRONEUSNICKNAME + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	}
	if(getUserByNick(nickname) != NULL) {
		response = IRC + ERR_NICKNAMEINUSENBR + nickname + ERR_NICKNAMEINUSE + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	}
	user->setNickName(nickname);
	if(user->isAuth() == true) {
		response = IRC + RPL_WELCOMENBR + user->getNickName() + RPL_WELCOME + user->getNickName() + "!" + user->getUserName() + "@*" + END;
		send(userFD, response.c_str(), response.size(), 0);
	}
}

