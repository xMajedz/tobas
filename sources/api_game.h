int GAME_get_game_frame(lua_State* L);

int GAME_get_reaction_time(lua_State* L);

int GAME_get_reaction_count(lua_State* L);

int luaopen_api_game(lua_State* L);

static const luaL_Reg api_game[] {
	{"get_game_frame", GAME_get_game_frame},
	{"get_reaction_time", GAME_get_reaction_time},
	{"get_reaction_count", GAME_get_reaction_count},
	{NULL, NULL},
};
