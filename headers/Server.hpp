#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP
# include "mainHeader.hpp"

class User;
class Channel;

class Server{
	private:
		int							_port;
		int							_sockfd;
		static bool					_signal;
		std::string					_password;
		std::vector<Channel>		_channels;
		std::vector<User*>			_Users;
		std::vector<struct pollfd>	_fds;

	public:
		void						listenUser(int userFD);
		void						acceptNewUser(void);
		void						clearUsers(int userFD);
		void						closeFds();
		void						freeUsers();
		bool						channelExists(const std::string &channelName) const;
		static void					handleSig(int signum);
		
		void		setupSocket();
		void		setupServer();
		
		Channel *			getChannel(const std::string& channelName);
		int					getPort();
		void				setPort(char *input);
		void				setPassword(char *input);
		std::string			getPassword();
		User*				getUserByFD(int userFD);
		User* 				getUserByNick(std::string nickName);
		std::vector<User*>	getUserVector();

		void						createChannel(std::string channelName);
		void						removeChannel(std::string channelName);
		void						selectOptions(std::string& buff, int userFD);
		std::vector<std::string>	parseOptions(std::string buff);
		void 						unknownCommand(std::string command, int userFD);

		void		cap(int userFD);
		void		join(std::vector<std::string>, int fd);
		void		privmsg(std::vector<std::string>, int fd);
		void		quit(std::vector<std::string>, int fd);
		void		nick(std::vector<std::string>, int fd);
		void		user(std::vector<std::string>, int fd);
		void		mode(std::vector<std::string>, int fd);
		void		topic(std::vector<std::string>, int fd);
		void		invite(std::vector<std::string>, int fd);
		void		kick(std::vector<std::string>, int fd);
		void		who(std::vector<std::string>, int fd);
		void		pass(std::vector<std::string>, int fd);
		void		part(std::vector<std::string>, int fd);
};

#endif