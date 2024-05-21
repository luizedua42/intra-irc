#include "../../headers/mainHeader.hpp"

void Server::quit(std::vector<std::string> options, int userFD) {
	std::string reason = options[0];
	User* user = Server::getUserByFD(userFD);
	std::string response = ":" + user->getNickName() + " QUIT :Quit: " + reason + "\r\n";
	send(userFD, response.c_str(), response.size(), 0);
	clearUsers(userFD);
	close(userFD);
	delete user;
}
