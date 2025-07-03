#include "api.h"
#include "game.h"

static int GAME_get_game_frame(lua_State* L)
{
	lua_pushnumber(L, Game::GetGameFrame());
	return 1;
}

static int GAME_get_reaction_time(lua_State* L)
{
	lua_pushnumber(L, Game::GetReactionTime());
	return 1;
}

static int GAME_get_reaction_count(lua_State* L)
{
	lua_pushnumber(L, Game::GetReactionCount());
	return 1;
}

static const luaL_Reg api_game[] {
	{"get_game_frame", GAME_get_game_frame},
	{"get_reaction_time", GAME_get_reaction_time},
	{"get_reaction_count", GAME_get_reaction_count},
	{NULL, NULL},
};

int luaopen_api_game(lua_State* L)
{
	luaL_register(L, "GAME", api_game);
	return 1;
}
