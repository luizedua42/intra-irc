#pragma once
#ifndef INCLUDES_HPP
# define INCLUDES_HPP

# include <algorithm>
# include <arpa/inet.h>
# include <csignal>
# include <cstdio>
# include <cstdlib>
# include <cstring>
# include <errno.h>
# include <fcntl.h>
# include <iostream>
# include <limits>
# include <list>
# include <map>
# include <netinet/in.h>
# include <poll.h>
# include <sstream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <vector>
# include "Channel.hpp"
# include "otherMacros.hpp"
# include "responses.hpp"
# include "Server.hpp"
# include "User.hpp"

void	splitString(std::string& str, char delimiter, std::vector<std::string>& tokens);

#endif
