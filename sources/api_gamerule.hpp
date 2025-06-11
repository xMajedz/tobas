#ifndef GAMERULE_H
#define GAMERULE_H

int api_get_game_frame(lua_State* L);

int api_get_reaction_time(lua_State* L);

int api_get_reaction_count(lua_State* L);

int luaopen_api_gamerule(lua_State* L);

static const luaL_Reg api_gamerule[] {
	{"get_game_frame", api_get_game_frame},
	{"get_reaction_time", api_get_reaction_time},
	{"get_reaction_count", api_get_reaction_count},
	{NULL, NULL},
};
#endif
