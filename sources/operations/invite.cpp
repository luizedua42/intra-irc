#include "../../headers/mainHeader.hpp"

void Server::invite(const std::vector<std::string> options, int userFD) {
	std::string response;
	User* user = getUserByFD(userFD);

    if (options.size() < 2) {
		response = IRC + ERR_NEEDMOREPARAMSNBR + " INVITE " + ERR_NEEDMOREPARAMS + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
    }

    const std::string& channelName = options[1];
    Channel* channelPtr = getChannel(channelName);

    if (channelPtr == NULL) {
		response = IRC + ERR_NOSUCHCHANNELNBR + "! " + channelName +  ERR_NOSUCHCHANNEL + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
    }

    if (!channelPtr->isUserOnChannel(user->getNickName())) {
		response = IRC + ERR_NOTONCHANNELNBR + channelName + " " + channelName + ERR_NOTONCHANNEL + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
    }

	std::istringstream iss(options[0]);
	std::string invitedUserNick;
	while (std::getline(iss, invitedUserNick, ',')) {
		User* invitedUser = getUserByNick(invitedUserNick);
		if (invitedUser == NULL) {
			response = IRC + ERR_NOSUCHNICKNBR + " " + channelName + " " + invitedUserNick + ERR_NOSUCHNICK + END;
			send(userFD, response.c_str(), response.size(), 0);
			continue;
		}

		if (channelPtr->isUserOnChannel(invitedUserNick))
		{
			response = IRC + ERR_USERONCHANNELNBR + channelName + " " + invitedUserNick + " " + channelName + ERR_USERONCHANNEL + END;
			send(userFD, response.c_str(), response.size(), 0);
			continue;
		}

		if (!channelPtr->isUserOperator(user->getNickName())) {
			response = IRC + ERR_CHANOPRIVSNEEDEDNBR + user->getNickName() + " " + channelName + ERR_CHANOPRIVSNEEDED + END;
			send(userFD, response.c_str(), response.size(), 0);
			return;
		}

		channelPtr->addToInviteList(invitedUserNick);
		response = IRC + RPL_INVITINGNBR + channelName + " " + invitedUserNick  + " " + channelName + END;
		send(userFD, response.c_str(), response.size(), 0);

		response = ":" + user->getNickName() + " INVITE " + invitedUserNick + " " + channelName + END;
		int invitedFD = invitedUser->getuserFD();
		send(invitedFD, response.c_str(), response.size(), 0);

		std::map<std::string, User*> operators = channelPtr->getOperators();
		for (std::map<std::string, User*>::iterator it = operators.begin(); it != operators.end(); ++it) {
			send(it->second->getuserFD(), response.c_str(), response.size(), 0);
		}
	}
}