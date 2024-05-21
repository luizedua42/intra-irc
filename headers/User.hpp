#pragma once
#ifndef User_HPP
# define User_HPP
# include "mainHeader.hpp"

class User {
	
	private:
		int			_userFD;
		std::string	_userIP;
		std::string	_nickName;
		std::string	_realName;
		std::string _userName;
		std::string _password;
	public:
		std::string	clientBuff;
		void setuserFD(int userFD);
		void setUserName(std::string userName);
		void setRealName(std::string realName);
		void setNickName(std::string nickName);
		void setuserIP(char * userIP);
		
		int			getuserFD();
		std::string	getRealName();
		std::string	getNickName();
		std::string getUserName();
		std::string	getuserIP();
		std::string	getPassword();
		void		setPassword(std::string password);
		bool		isAuth();

		User(const int userFD);
};

#endif