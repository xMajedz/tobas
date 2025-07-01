#include "luau.h"
#include "api.h"
#include "api_game.h"

extern API api;

int GAME_get_game_frame(lua_State* L)
{
	lua_pushnumber(L, api.game->state.game_frame);
	return 1;
}

int GAME_get_reaction_time(lua_State* L)
{
	lua_pushnumber(L, api.game->rules.reaction_time);
	return 1;
}

int GAME_get_reaction_count(lua_State* L)
{
	lua_pushnumber(L, api.game->state.reaction_count);
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
