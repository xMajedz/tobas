#include <api_gamerule.hpp>

int api_get_game_frame(lua_State* L)
{
	lua_pushnumber(L, game.state.game_frame);
	return 1;
}

int api_get_reaction_time(lua_State* L)
{
	lua_pushnumber(L, game.rules.reaction_time);
	return 1;
}

int api_get_reaction_count(lua_State* L)
{
	lua_pushnumber(L, game.state.reaction_count);
	return 1;
}

int luaopen_api_gamerule(lua_State* L)
{
	luaL_register(L, "GAME", api_gamerule);
	return 1;
}
