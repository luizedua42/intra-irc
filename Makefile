NAME	= ircserv
ECHO	= echo
YELLOW	= \033[1;33m
GREEN	= \033[1;32m
RED		= \033[0;31m
RESET	= \033[0m
C++		= c++
CFLAGS	= -Wall -Werror -Wextra -g3 -std=c++98

HEADERS = ./headers/mainHeader.hpp\
		./headers/Channel.hpp\
		./headers/otherMacros.hpp\
		./headers/responses.hpp\
		./headers/Server.hpp\
		./headers/User.hpp\

SOURCES = ./sources/main.cpp\
		./sources/operations/cap.cpp\
		./sources/operations/invite.cpp\
		./sources/operations/join.cpp\
		./sources/operations/kick.cpp\
		./sources/operations/mode.cpp\
		./sources/operations/nick.cpp\
		./sources/operations/Oper.cpp\
		./sources/operations/part.cpp\
		./sources/operations/pass.cpp\
		./sources/operations/privmsg.cpp\
		./sources/operations/quit.cpp\
		./sources/operations/topic.cpp\
		./sources/operations/user.cpp\
		./sources/operations/who.cpp\
		./sources/Channel.cpp\
		./sources/Server.cpp\
		./sources/User.cpp\
		./sources/utils.cpp\

OBJ_DIR = ./mandatory/obj
OBJ_FILES = $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)
INCLUDE = -I ./headers

all: ${NAME}

${NAME}: ${OBJ_FILES}
	@${ECHO} "${YELLOW}Linking $@...${RESET}"
	@$(C++) -g3 ${CFLAGS} $^ -o $@
	@${ECHO} "${GREEN}Linking complete.${RESET}"

$(OBJ_DIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	@$(C++) $(CFLAGS) -c $< -o $@ $(INCLUDE)

clean:
	@rm -fr ${OBJ_DIR}
	@rmdir ./mandatory
	@${ECHO} "${RED}Objects removed!${RESET}"

fclean: clean
	@rm -fr ${NAME}
	@${ECHO} "${RED}Files removed!${RESET}"

re: fclean ${NAME}

.PHONY: all clean fclean re
