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

static int Game_GetGamerules(lua_State* L)
{
	auto rules = Game::GetGamerules();
	lua_newtable(L);
	lua_pushstring(L, rules.mod.data());
	lua_setfield(L, -2, "mod");
	lua_newtable(L);
	lua_pushnumber(L, rules.gravity.x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, rules.gravity.y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, rules.gravity.z);
	lua_setfield(L, -2, "z");
	lua_setfield(L, -2, "gravity");
	lua_pushnumber(L, rules.numplayers);
	lua_setfield(L, -2, "numplayers");
	lua_pushnumber(L, rules.turnframes);
	lua_setfield(L, -2, "turnframes");
	lua_pushnumber(L, rules.max_contacts);
	lua_setfield(L, -2, "max_contacts");
	lua_pushnumber(L, rules.reaction_time);
	lua_setfield(L, -2, "reaction_time");
	lua_pushnumber(L, rules.engagedistance);
	lua_setfield(L, -2, "engagedistance");
	lua_pushnumber(L, rules.engageheight);
	lua_setfield(L, -2, "engageheight");
	lua_pushnumber(L, rules.friction);
	lua_setfield(L, -2, "friction");
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

	{"GetGamerules", Game_GetGamerules},

	{NULL, NULL},
};

int luaopen_api_game(lua_State* L)
{
	luaL_register(L, "Game", api_game);
	return 1;
}
