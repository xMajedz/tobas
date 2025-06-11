#include <game.hpp>

Gamerules Game::GetGamerules()
{
	return rules;
}

std::map<std::string, Player> Game::GetPlayers()
{
	return players;
}

std::map<std::string, Body> Game::GetObjects()
{
	return objects;
}

void Game::TogglePause()
{
	state.pause = state.pause == false;
}

bool Game::GetPause()
{
	return state.pause;
}

bool Game::GetFreeze()
{
	return state.freeze;
}
