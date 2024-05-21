#include "../../headers/mainHeader.hpp"

namespace mode {
	void setOp(Channel* channel, std::string user) {
		channel->promoteToOperator(user);
	}
	void unsetOp(Channel* channel, std::string user) {
		channel->demoteFromOperator(user);
	}
	void setTopic(Channel* channel) {
		channel->setMode("t", true);
	}
	void unsetTopic(Channel* channel) {
		channel->setMode("t", false);
	}
	void setInvite(Channel* channel) {
		channel->setMode("i", true);
	}
	void unsetInvite(Channel* channel) {
		channel->setMode("i", false);
	}
	void setKey(Channel* channel, std::string key) {
		channel->setMode("k", true);
		channel->setPassword(key);
	}
	void unsetKey(Channel* channel) {
		std::string key = "";
		channel->setMode("k", false);
		channel->setPassword(key);
	}
	void setLimit(Channel* channel, std::string param) {
		channel->setMode("l", true);
		std::stringstream ss(param);
		int limit;
		ss >> limit;
		channel->setUserLimit(limit);
	}
	void unsetLimit(Channel* channel) {
		channel->setMode("l", false);
		channel->setUserLimit(MAX_USERS);
	}
}

void Server::mode(std::vector<std::string> options, int userFD) {
	std::string response;
	std::string mode;
	
	User* user = getUserByFD(userFD);
	std::string channelName = options[0];
	Channel* channel = getChannel(channelName);
	if(channel == NULL) {
		response = IRC + ERR_NOSUCHCHANNELNBR + user->getNickName() + " " + channelName + ERR_NOSUCHCHANNEL + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	}
	if(options.size() == 1 || options[1] == "") {
		std::string modes = channel->getAllModes();
		response = IRC + RPL_CHANNELMODEISNBR + user->getNickName() + " " + channelName + " " + modes + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	}

	int i = 0;
	std::string modes[] = {"-i", "+i", "-t", "+t", "-k", "+k", "-o", "+o", "-l", "+l"};
	mode = options[1].substr(0, options[1].find('\r'));
	std::string modeParam = "";
	if(options.size() > 2)
		modeParam = options[2];
	User* paramUser = getUserByNick(modeParam);

	response = ":" + user->getNickName() + "!" + user->getUserName() + "@ft.irc MODE " + channelName + " ";

	for(; i < 10; i++) {
		if(mode == modes[i])
			break;
	}

	switch (i) {
		case 0:
			mode::unsetInvite(channel);
			response += "-i";
			break;

		case 1:
			mode::setInvite(channel);
			response += "+i";
			break;

		case 2:
			mode::unsetTopic(channel);
			response += "-t";
			break;

		case 3:
			mode::setTopic(channel);
			response += "+t";
			break;

		case 4:
			mode::unsetKey(channel);
			response += "-k";
			break;

		case 5:
			if (modeParam == "") {
				response = IRC + ERR_NEEDMOREPARAMSNBR + user->getNickName() + channelName + " " + channelName + " k * :You must specify a parameter for the key mode. Syntax: <key>." + END;
				send(userFD, response.c_str(), response.size(), 0);
				return;
			}

			mode::setKey(channel, modeParam);
			response += "+k";
			break;

		case 6:
			if (modeParam == "") {
				response = IRC + ERR_NEEDMOREPARAMSNBR + user->getNickName() + channelName + " " + channelName +  " o * :You must specify a parameter for the nick mode. Syntax: <nick>." + END;
				send(userFD, response.c_str(), response.size(), 0);
				return;
			}

			if (!channel->isUserOperator(user->getNickName())) {
				response = IRC + ERR_CHANOPRIVSNEEDEDNBR + user->getNickName() + " " + channelName + ERR_CHANOPRIVSNEEDED + END;
				send(userFD, response.c_str(), response.size(), 0);
				return;
			}

			if (paramUser == NULL) {
				response = IRC + ERR_NOSUCHNICKNBR + modeParam + ERR_NOSUCHNICK + END;
				send(userFD, response.c_str(), response.size(), 0);
				return;
			}

			if (!channel->isUserOnChannel(paramUser->getNickName()) || !channel->isUserOperator(paramUser->getNickName())) {
				return;
			}

			mode::unsetOp(channel, modeParam);
			response += "-o";
			break;

		case 7:
			if (modeParam == "") {
				response = IRC + ERR_NEEDMOREPARAMSNBR + user->getNickName() + channelName + " " + channelName +  " o * :You must specify a parameter for the nick mode. Syntax: <nick>." + END;
				send(userFD, response.c_str(), response.size(), 0);
				return;
			}

			if (!channel->isUserOperator(user->getNickName())) {
				response = IRC + ERR_CHANOPRIVSNEEDEDNBR + user->getNickName() + " " + channelName + ERR_CHANOPRIVSNEEDED + END;
				send(userFD, response.c_str(), response.size(), 0);
				return;
			}

			if (paramUser == NULL) {
				response = IRC + ERR_NOSUCHNICKNBR + modeParam + ERR_NOSUCHNICK + END;
				send(userFD, response.c_str(), response.size(), 0);
				return;
			}

			if (!channel->isUserOnChannel(paramUser->getNickName()) || channel->isUserOperator(paramUser->getNickName())) {
				return;
			}

			mode::setOp(channel, modeParam);
			response += "+o";
			break;

		case 8:
			mode::unsetLimit(channel);
			response += "-l";
			break;

		case 9:
			if (modeParam == "") {
				response = IRC + ERR_NEEDMOREPARAMSNBR + user->getNickName() + channelName + " " + channelName +  " l * :You must specify a parameter for the limit mode. Syntax: <limit>." + END;
				send(userFD, response.c_str(), response.size(), 0);
				return;
			}

			mode::setLimit(channel, modeParam);
			response += "+l";
			break;

		default:
			response = "Invalid mode";
			break;
	}

	response += " " + modeParam + END;
	send(userFD, response.c_str(), response.size(), 0);
}
