#include "api.h"
#include "game.h"
#include "netcode_server.h"

Window window;

int main()
{
	SetTraceLogLevel(LOG_ERROR);

	Game::Init();
	//Game::NewGame();
	host_game();
	host_close();
	Game::Quit();
}
