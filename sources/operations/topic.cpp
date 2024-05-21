#include "../../headers/mainHeader.hpp"

void Server::topic(std::vector<std::string> options, int userFD) {
	std::string response;
	
	std::string channelName = options[0];
	
	Channel* channelPtr = getChannel(channelName);
	if (channelPtr == NULL) {
		response = IRC + ERR_NOSUCHCHANNELNBR + channelName + ERR_NOSUCHCHANNEL + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	}
	User* user = Server::getUserByFD(userFD);

	if (options.size() == 1) {
		if(channelPtr->getTopic().size() == 0) {
			response = IRC + RPL_NOTOPICNBR + user->getNickName() + " " + channelName + RPL_NOTOPIC + END;
			if(send (userFD, response.c_str(), response.size(), 0) == -1)
				std::cerr << "Error sending message" << std::endl;
			return;
		}
		
		response = IRC + RPL_TOPICNBR + user->getNickName() + " " + channelName + " :" + channelPtr->getTopic() + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	} else if (options.size() > 1) {
		std::string topic = options[1].substr(1);

		if(channelPtr->getMode("t") == true && !channelPtr->isUserOperator(user->getNickName())) {
			response = IRC + ERR_CHANOPRIVSNEEDEDNBR + user->getNickName() + " " + channelName + ERR_CHANOPRIVSNEEDED + END;
			send(userFD, response.c_str(), response.size(), 0);
			return;
		}

		channelPtr->setTopic(topic);
		
		std::vector<User *> users = channelPtr->getAllUsers();

		response = ":" + user->getNickName() + "!~" + user->getRealName() + "@ft.irc TOPIC " + channelName + " " + topic + END;
		
		for (size_t i = 0; i < users.size(); i++) {
			send(users[i]->getuserFD(), response.c_str(), response.size(), 0);
		}
	} else {
		response = IRC + ERR_NEEDMOREPARAMSNBR + " TOPIC " + ERR_NEEDMOREPARAMS + END;		
		send(userFD, response.c_str(), response.size(), 0);
	}
}
