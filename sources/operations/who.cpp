#include "../../headers/mainHeader.hpp"

void Server::who(std::vector<std::string> options, int userFD) {
	std::string response;

	std::string channelName = options[0];
	Channel* channelPtr = getChannel(channelName);

	std::vector<User *> users = channelPtr->getAllUsers();
	for (size_t i = 0; i < users.size(); i++) {
		std::string flags = "H ";
		if (channelPtr->isUserOperator(users[i]->getNickName()))
			flags += "@ ";
		response = IRC + RPL_WHOREPLYNBR + channelName + " " + users[i]->getUserName() + "42sp.org.br ft_irc " + 
			users[i]->getNickName() + " " + flags + ":0" + users[i]->getRealName() + END;
		std::cout << response << std::endl;
		if(send (userFD, response.c_str(), response.size(), 0) == -1)
			std::cerr << "Error sending message" << std::endl;
	}
	response = IRC + RPL_ENDOFWHONBR + channelName + RPL_ENDOFWHO + END;
	if(send (userFD, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Error sending message" << std::endl;
}
