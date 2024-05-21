#include "../../headers/mainHeader.hpp"

std::vector<std::string> Server::parseOptions(std::string str) {
	std::string word;
	std::stringstream ss(str);
	std::vector<std::string> splitted;
	while (std::getline(ss, word, ' ')) {
		if(word.find('\r') != std::string::npos)
			splitted.push_back(word.substr(0, word.find('\r')));
		else
			splitted.push_back(word);
	}
	return splitted;
}

std::vector<std::string> splitBuff(std::string buff) {
	std::vector<std::string> splittedBuff;
	std::string word;
	std::stringstream ss(buff);
	while (std::getline(ss, word, '\n')) {
		splittedBuff.push_back(word);
	}
	return splittedBuff;
}

void Server::unknownCommand(std::string command, int userFD) {
	std::string response = command + ":Unknown command\r\n";
	if(send(userFD, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Error sending message" << std::endl;
}

void Server::selectOptions(std::string& buff, int userFD) {
	std::vector<std::string> splittedBuff = splitBuff(buff);
	User* client = Server::getUserByFD(userFD);
	std::string requests[] = {"CAP", "USER", "NICK", "JOIN", "PRIVMSG", "QUIT", "MODE", "TOPIC", "INVITE", "KICK", "WHO", "PASS", "PART"};

	do {
		int i = 0;
		std::string options = splittedBuff[0].substr(0, splittedBuff[0].find_first_of(" "));
		std::cout << "buff: " << buff << std::endl;
		std::cout << "client: " << userFD << std::endl;
		for(; i < 13; i++) {
			if(options == requests[i])
				break;
		}
		if(client->isAuth() == false) {
			if (i != 0 && i != 1 && i != 2 && i != 11) {
				std::string response = "You have not registered\r\n";
				send(userFD, response.c_str(), response.size(), 0);
				client->clientBuff.clear();
				return;
			}
		}
		std::string parsedOptions = splittedBuff[0].substr(splittedBuff[0].find_first_of(" ") + 1);
		switch (i) {
			case 0:
				cap(userFD);
				break;
			case 1:
				user(parseOptions(parsedOptions), userFD);
				break;
			case 2:
				nick(parseOptions(parsedOptions), userFD);
				break;
			case 3:
				join(parseOptions(parsedOptions), userFD);
				break;
			case 4:
				privmsg(parseOptions(parsedOptions), userFD);
				break;
			case 5:
				client->clientBuff.clear();
				quit(parseOptions(parsedOptions), userFD);
				splittedBuff.erase(splittedBuff.begin());
				return;
				break;
			case 6:
				mode(parseOptions(parsedOptions), userFD);
				break;
			case 7:
				topic(parseOptions(parsedOptions), userFD);
				break;
			case 8:
				invite(parseOptions(parsedOptions), userFD);
				break;
			case 9:
				kick(parseOptions(parsedOptions), userFD);
				break;
			case 10:
				who(parseOptions(parsedOptions), userFD);
				break;
			case 11:
				pass(parseOptions(parsedOptions), userFD);
				break;
			case 12:
				part(parseOptions(parsedOptions), userFD);
				break;
			default:
				unknownCommand(options, userFD);
				break;
		}
		splittedBuff.erase(splittedBuff.begin());
	} while (!splittedBuff.empty());
	client->clientBuff.clear();
}
