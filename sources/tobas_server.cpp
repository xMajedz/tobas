#include "api.h"
#include "netcode_server.h"

Window window;

int main()
{
	SetTraceLogLevel(LOG_ERROR);
	Server::HostGame();
	Server::Close();
}
