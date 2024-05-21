#include "../../headers/mainHeader.hpp"

void Server::kick(std::vector<std::string> options, int userFD) {
	User* user = getUserByFD(userFD);
	std::string response;

    if (options.size() < 2) {
		response = IRC + ERR_NEEDMOREPARAMSNBR + " KICK " + ERR_NEEDMOREPARAMS + END;
		return;
    }

	std::string kickReason;
    const std::string& channelName = options[0];
	
	if (options.size() == 3) { 
		kickReason = options[2].substr(1);
	} else {
		kickReason = "no reason";
	}

    Channel* channelPtr = getChannel(channelName);
    if (channelPtr == NULL) {
		response = IRC + ERR_NOSUCHCHANNELNBR + channelName + " " + channelName + ERR_NOSUCHCHANNEL + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
    }

    if (!channelPtr->isUserOperator(user->getNickName())) {
		response = IRC + ERR_CHANOPRIVSNEEDEDNBR + user->getNickName() + " " + channelName + ERR_CHANOPRIVSNEEDED + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
    }

	std::istringstream iss(options[1]);
	std::string userToBeKickedNick;
	while (std::getline(iss, userToBeKickedNick, ',')) {
		User* userToBeKicked = getUserByNick(userToBeKickedNick);
		if (userToBeKicked == NULL) {
			response = IRC + ERR_NOSUCHNICKNBR + " " + channelName + " " + userToBeKickedNick + ERR_NOSUCHNICK + END;
			send(userFD, response.c_str(), response.size(), 0);
			continue;
		}

		if (!channelPtr->isUserOnChannel(userToBeKickedNick))
		{
			response = IRC + ERR_USERNOTINCHANNELNBR + channelName + " " + userToBeKickedNick + " " + channelName + ERR_USERNOTINCHANNEL + END;
			send(userFD, response.c_str(), response.size(), 0);
			continue;
		}

		if (!channelPtr->isUserOperator(user->getNickName())) {
			response = IRC + ERR_CHANOPRIVSNEEDEDNBR + user->getNickName() + " " + channelName + ERR_CHANOPRIVSNEEDED + END;
			send(userFD, response.c_str(), response.size(), 0);
			return;
		}

		std::vector<User *> users = channelPtr->getAllUsers();
		response = ":" + user->getNickName() + "!" + user->getRealName() + "@ft.irc KICK " + channelName + " " + userToBeKickedNick + " :"+ kickReason + END;
		for (size_t i = 0; i < users.size(); i++) {
			send(users[i]->getuserFD(), response.c_str(), response.size(), 0);
		}
		channelPtr->removeUser(userToBeKickedNick);
	}
}