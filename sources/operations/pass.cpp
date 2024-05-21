#include "../../headers/mainHeader.hpp"

void Server::pass(std::vector<std::string> options, int userFD) {
	User* user = Server::getUserByFD(userFD);
	std::string response;
	std::string password = options[0].substr(0, options[0].find('\r'));

	if(options.size() != 1) {

		response = IRC + ERR_NEEDMOREPARAMSNBR + ERR_NEEDMOREPARAMS + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	}

	if(!user->getPassword().empty()) {

		response = IRC + ERR_ALREADYREGISTEREDNBR + user->getUserName() + ERR_ALREADYREGISTERED + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	}

	user->setPassword(password);

	if (user->getPassword() != Server::getPassword()) {
		response = IRC + ERR_PASSWDMISMATCHNBR + ERR_PASSWDMISMATCH + END;;
		send(userFD, response.c_str(), response.size(), 0);
		close(userFD);
		User* user = Server::getUserByFD(userFD);
		Server::clearUsers(userFD);
		delete user;
		return;
	}
}
