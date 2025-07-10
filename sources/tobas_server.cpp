#include "api.h"
#include "netcode_server.h"

int main()
{
	SetTraceLogLevel(LOG_ERROR);
	Server::HostGame();
	Server::Close();
}
