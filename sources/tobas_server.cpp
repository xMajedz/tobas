#include "game.h"
#include "netcode_server.h"

Console console;
Window window;

int main()
{
	Game::Init();
	Game::NewGame();
	host_game();
	host_close;
	Game::Quit();
}
