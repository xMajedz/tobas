#include "api.h"
#include "game.h"

static int Game_GetMod(lua_State* L)
{
	lua_pushstring(L, Game::GetMod());
	return 1;
}

static int Game_GetGameFrame(lua_State* L)
{
	lua_pushnumber(L, Game::GetGameFrame());
	return 1;
}

static int Game_GetReactionTime(lua_State* L)
{
	lua_pushnumber(L, Game::GetReactionTime());
	return 1;
}

static int Game_GetReactionCount(lua_State* L)
{
	lua_pushnumber(L, Game::GetReactionCount());
	return 1;
}

static const luaL_Reg api_game[] {
	{"GetMod", Game_GetMod},
	{"GetGameFrame", Game_GetGameFrame},
	{"GetReactionTime", Game_GetReactionTime},
	{"GetReactionCount", Game_GetReactionCount},
	{NULL, NULL},
};

int luaopen_api_game(lua_State* L)
{
	luaL_register(L, "Game", api_game);
	return 1;
}
