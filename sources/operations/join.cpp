#include "../../headers/mainHeader.hpp"

static bool isValidChannelName(const std::string& channelName) {
    if (channelName.empty() || channelName[0] != '#') {
        return false;
    }

    for (size_t i = 1; i < channelName.size(); i++) {
        if (!isalnum(channelName[i]) && channelName[i] != '-' && channelName[i] != '_' && channelName[i] != '+') {
            return false;
        }
    }

    return true;
}

void Server::join(std::vector<std::string> options, int userFD) {
	bool isUserOperator = false;

	std::string response;
	std::vector<std::string> channels;
    splitString(options[0], ',', channels);

    std::vector<std::string> passwords;
    if (options.size() > 1) {
        splitString(options[1], ',', passwords);
    }

	User* user = getUserByFD(userFD);
	for (size_t i = 0; i < channels.size(); i++) {
		if (!isValidChannelName(channels[i])) {
			response = IRC + ERR_BADCHANMASKNBR + channels[i] + ERR_BADCHANMASK + END;
			send(userFD, response.c_str(), response.size(), 0);
			continue;
		}

		if (!channelExists(channels[i])) {
			createChannel(channels[i]);
			isUserOperator = true;
		}

		Channel* channelPtr = getChannel(channels[i]);
		if (channelPtr->getMode("i") == true) {
			if (!channelPtr->isUserInvited(user->getNickName())) {
				response = IRC + ERR_INVITEONLYCHANNBR + channels[i] + " " + channels[i] + ERR_INVITEONLYCHAN + END;
				send(userFD, response.c_str(), response.size(), 0);
				continue;
			} else {
				channelPtr->removeFromInviteList(user->getNickName());
			}
		}

		std::string password = channelPtr->getPassword();
		if (!password.empty() && passwords.size() < i + 1) {
			response = IRC + ERR_BADCHANNELKEYNBR + channels[i] + " " + channels[i] + ERR_BADCHANNELKEY + END;
			send(userFD, response.c_str(), response.size(), 0);
			continue;
		}
		if (!password.empty() && password != passwords[i]) {
			response = IRC + ERR_PASSWDMISMATCHNBR + channels[i] + ERR_PASSWDMISMATCH + END;
			send(userFD, response.c_str(), response.size(), 0);
			continue;
		}

		if(channelPtr->getUserCount() < channelPtr->getUserLimit()) {
			channelPtr->addUser(user);
		} else {
			response = IRC + ERR_CHANNELISFULLNBR + channels[i] + " " + channels[i] + ERR_CHANNELISFULL + END;
			send(userFD, response.c_str(), response.size(), 0);
			continue;
		}
		if (isUserOperator) {
			channelPtr->promoteToOperator(user->getNickName());
		}
		
		std::cout << " Joining channel: " << channels[i] << std::endl;
		std::string response = ":" + user->getNickName() + " JOIN " + channels[i] + END;
		if(channelPtr->getTopic() != "")
			response += ":" + user->getNickName() + " TOPIC " + channels[i] + " :" + channelPtr->getTopic() + END;
		send(userFD, response.c_str(), response.size(), 0);
	}
}
