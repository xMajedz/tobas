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

int luaopen_api_game(lua_State* L)
{
	luaL_register(L, "GAME", api_game);
	return 1;
}
