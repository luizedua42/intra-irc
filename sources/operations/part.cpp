#include "../../headers/mainHeader.hpp"

void Server::part(const std::vector<std::string> options, int userFD) {
	User* user = getUserByFD(userFD);
	std::string partReason = "";
	std::string response;

    if (options.empty()) {
		response = IRC + ERR_NEEDMOREPARAMSNBR + " PART " + ERR_NEEDMOREPARAMS + END;
		return;
    } else if (options.size() > 1) {
		for (size_t i = 1; i < options.size(); i++) {
			partReason += options[i];
		}
	}

    std::istringstream iss(options[0]);
    std::string channelName;

	while (std::getline(iss, channelName, ',')) {
		Channel* channelPtr = getChannel(channelName);
		if (channelPtr == NULL) {
			response = IRC + ERR_NOSUCHCHANNELNBR + channelName + ERR_NOSUCHCHANNEL + END;
			send(userFD, response.c_str(), response.size(), 0);
		} else {
			response = ":" + user->getNickName() + "!~" + user->getRealName() + "@* PART " + channelName + " :" + partReason + END;
			std::vector<User *> users = channelPtr->getAllUsers();
			for (size_t i = 0; i < users.size(); i++) {
				send(users[i]->getuserFD(), response.c_str(), response.size(), 0);
			}
		}
		channelPtr->removeUser(user->getNickName());

		if (channelPtr->getAllUsers().size() == 0) {
			removeChannel(channelName);
		}
	}
	
}
