#include "api.h"
#include "game.h"

static int Game_NewGame(lua_State* L)
{
	Game::NewGame();
	return 1;
}

static int Game_Step(lua_State* L)
{
	Game::Step(lua_tointeger(L, -1));
	return 1;
}

static int Game_EnterMode(lua_State* L)
{
	Game::EnterMode((Gamemode)lua_tointeger(L, -1));
	return 1;
}

static int Game_ToggleGhosts(lua_State* L)
{
	Game::ToggleGhosts();
	return 1;
}

static int Game_TogglePause(lua_State* L)
{
	Game::TogglePause();
	return 1;
}

static int Game_GetFreeze(lua_State* L)
{
	lua_pushboolean(L, Game::GetFreeze());
	return 1;
}


static int Game_GetPause(lua_State* L)
{
	lua_pushboolean(L, Game::GetPause());
	return 1;
}

static int Game_IsMode(lua_State* L)
{
	lua_pushboolean(L, Game::GetGamemode() == (Gamemode)lua_tointeger(L, -1));
	return 1;
}

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

static int Game_GetSelectedPlayerID(lua_State* L)
{
	lua_pushinteger(L, Game::GetSelectedPlayerID());
	return 1;
}

static int Game_IsSelectedPlayerValid(lua_State* L)
{
	lua_pushboolean(L, Game::GetSelectedPlayerID() != -1);
	return 1;
}

static int Game_Refreeze(lua_State* L)
{
	Game::Refreeze();
	return 1;
}

static int Game_TogglePlayerPassiveStatesAlt(lua_State* L)
{
	Game::TogglePlayerPassiveStatesAlt(lua_tointeger(L, -1));
	return 1;
}

static int Game_TogglePlayerPassiveStates(lua_State* L)
{
	Game::TogglePlayerPassiveStates(lua_tointeger(L, -1));
	return 1;
}

static int Game_GetSelectedJointID(lua_State* L)
{
	lua_pushinteger(L, Game::GetSelectedJointID());
	return 1;
}

static int Game_IsSelectedJointValid(lua_State* L)
{
	lua_pushboolean(L, Game::GetSelectedJointID() != -1);
	return 1;
}

static int Game_ToggleJointActiveStateAlt(lua_State* L)
{
	Game::ToggleJointActiveStateAlt(lua_tointeger(L, -1));
	return 1;
}
static int Game_ToggleJointActiveState(lua_State* L)
{
	Game::ToggleJointActiveState(lua_tointeger(L, -1));
	return 1;
}
static int Game_ToggleJointPassiveStateAlt(lua_State* L)
{
	Game::ToggleJointPassiveStateAlt(lua_tointeger(L, -1));
	return 1;
}
static int Game_ToggleJointPassiveState(lua_State* L)
{
	Game::ToggleJointPassiveState(lua_tointeger(L, -1));
	return 1;
}
static int Game_CycleJointStateAlt(lua_State* L)
{
	Game::CycleJointStateAlt(lua_tointeger(L, -1));
	return 1;
}
static int Game_CycleJointState(lua_State* L)
{
	Game::CycleJointState(lua_tointeger(L, -1));
	return 1;
}

static int Game_ToggleSelectedPlayerPassiveStatesAlt(lua_State* L)
{
	Game::ToggleSelectedPlayerPassiveStatesAlt();
	return 1;
}

static int Game_ToggleSelectedPlayerPassiveStates(lua_State* L)
{
	Game::ToggleSelectedPlayerPassiveStates();
	return 1;
}

static int Game_ToggleSelectedJointActiveStateAlt(lua_State* L)
{
	Game::ToggleSelectedJointActiveStateAlt();
	return 1;
}
static int Game_ToggleSelectedJointActiveState(lua_State* L)
{
	Game::ToggleSelectedJointActiveState();
	return 1;
}
static int Game_ToggleSelectedJointPassiveStateAlt(lua_State* L)
{
	Game::ToggleSelectedJointPassiveStateAlt();
	return 1;
}
static int Game_ToggleSelectedJointPassiveState(lua_State* L)
{
	Game::ToggleSelectedJointPassiveState();
	return 1;
}
static int Game_CycleSelectedJointStateAlt(lua_State* L)
{
	Game::CycleSelectedJointStateAlt();
	return 1;
}
static int Game_CycleSelectedJointState(lua_State* L)
{
	Game::CycleSelectedJointState();
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

static const luaL_Reg api_game[]
{
	{"NewGame", Game_NewGame},

	{"Step", Game_Step},

	{"EnterMode", Game_EnterMode},

	{"TogglePause", Game_TogglePause},
	{"ToggleGhosts", Game_ToggleGhosts},

	{"TogglePlayerPassiveStates", Game_TogglePlayerPassiveStates},
	{"TogglePlayerPassiveStatesAlt", Game_TogglePlayerPassiveStatesAlt},

	{"ToggleSelectedPlayerPassiveStatesAlt", Game_ToggleSelectedPlayerPassiveStatesAlt},
	{"ToggleSelectedPlayerPassiveStates", Game_ToggleSelectedPlayerPassiveStates},

	{"ToggleJointActiveStateAlt", Game_ToggleJointActiveStateAlt},
	{"ToggleJointActiveState", Game_ToggleJointActiveState},
	{"ToggleJointPassiveStateAlt", Game_ToggleJointPassiveStateAlt},
	{"ToggleJointPassiveState", Game_ToggleJointPassiveState},

	{"CycleJointStateAlt", Game_CycleJointStateAlt},
	{"CycleJointState", Game_CycleJointState},

	{"ToggleSelectedJointActiveStateAlt", Game_ToggleSelectedJointActiveStateAlt},
	{"ToggleSelectedJointActiveState", Game_ToggleSelectedJointActiveState},
	{"ToggleSelectedJointPassiveStateAlt", Game_ToggleSelectedJointPassiveStateAlt},
	{"ToggleSelectedJointPassiveState", Game_ToggleSelectedJointPassiveState},

	{"CycleSelectedJointStateAlt", Game_CycleSelectedJointStateAlt},
	{"CycleSelectedJointState", Game_CycleSelectedJointState},

	{"Refreeze", Game_Refreeze},

	{"IsPause", Game_GetPause},
	{"IsFreeze", Game_GetFreeze},

	{"IsMode", Game_IsMode},

	{"IsSelectedPlayerValid", Game_IsSelectedPlayerValid},
	{"IsSelectedJointValid", Game_IsSelectedJointValid},

	{"GetMod", Game_GetMod},
	{"GetGameFrame", Game_GetGameFrame},
	{"GetReactionTime", Game_GetReactionTime},
	{"GetReactionCount", Game_GetReactionCount},
	{"GetObjectCount", Game_GetObjectCount},
	{"GetPlayerCount", Game_GetPlayerCount},
	{"GetPlayerBodyCount", Game_GetPlayerBodyCount},
	{"GetPlayerJointCount", Game_GetPlayerJointCount},
	{"GetGamerules", Game_GetGamerules},

	{"GetSelectedPlayerID", Game_GetSelectedPlayerID},
	{"GetSelectedJointID", Game_GetSelectedJointID},

	{NULL, NULL},
};

int luaopen_api_game(lua_State* L)
{
	luaL_register(L, "Game", api_game);
	lua_getglobal(L, "Game");
	lua_pushinteger(L, Gamemode::FREE_PLAY);
	lua_setfield(L, -2, "MODE_FREEPLAY");
	lua_pushinteger(L, Gamemode::SELF_PLAY);
	lua_setfield(L, -2, "MODE_SELFPLAY");
	lua_pushinteger(L, Gamemode::REPLAY_PLAY);
	lua_setfield(L, -2, "MODE_REPLAY");
	lua_pop(L, 1);
	return 1;
}
