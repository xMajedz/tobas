#include "api.h"
#include "netcode_server.h"

#include <iostream>

int main()
{
	SetTraceLogLevel(LOG_ERROR);
	Server::HostGameThread();
	char input;
	bool running = true;
	while (running) {
		std::cin >> input;
		std::cout << input << std::endl;
		if (input == 'q')
			running = false;
	}
	Server::CloseThread();
}
