#include "../headers/mainHeader.hpp"

int main(int ac, char **av) {
	if (ac != 3){
		std::cout << "Usage: " << av[0] << " <port> <password>"  << std::endl;
		return 1;
	}

	Server server;
	server.setPort(av[1]);
	server.setPassword(av[2]);

	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, server.handleSig);
	signal(SIGQUIT, server.handleSig);
	try {
		server.setupServer();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	std::cout << "Server is closing..." << std::endl;
	return 0;
}
