#include "api.h"
#include "game.h"

static int Game_GetMod(lua_State* L)
{
	auto mod = Game::GetMod();
	lua_pushlstring(L, mod.data(), mod.size());
	return 1;
}

static int Game_GetGameFrame(lua_State* L)
{
	lua_pushinteger(L, Game::GetGameFrame());
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

static int Game_GetObjectCount(lua_State* L)
{
	lua_pushnumber(L, Game::GetObjectCount());
	return 1;
}

static int Game_GetPlayerCount(lua_State* L)
{
	lua_pushnumber(L, Game::GetPlayerCount());
	return 1;
}

static int Game_GetPlayerBodyCount(lua_State* L)
{
	lua_pushnumber(L, Game::GetPlayerBodyCount(lua_tointeger(L, -1)));
	return 1;
}

static int Game_GetPlayerJointCount(lua_State* L)
{
	lua_pushnumber(L, Game::GetPlayerJointCount(lua_tointeger(L, -1)));
	return 1;
}

static const luaL_Reg api_game[] {
	{"GetMod", Game_GetMod},
	{"GetGameFrame", Game_GetGameFrame},
	{"GetReactionTime", Game_GetReactionTime},
	{"GetReactionCount", Game_GetReactionCount},
	{"GetObjectCount", Game_GetObjectCount},
	{"GetPlayerCount", Game_GetPlayerCount},
	{"GetPlayerBodyCount", Game_GetPlayerBodyCount},
	{"GetPlayerJointCount", Game_GetPlayerJointCount},
	{NULL, NULL},
};

int luaopen_api_game(lua_State* L)
{
	luaL_register(L, "Game", api_game);
	return 1;
}
