#include <api_callbacks.h>

#include <iostream>
#define PRINT(X) std::cout << "-- TEST " << X << " --" << std::endl;
#define PRINT_A PRINT("A")
#define PRINT_B PRINT("B")
#define PRINT_C PRINT("C")

const char* events[] = {
	"Console",
	"NewGame",
	"Freeze",
	"Step",
	"NearCallback",
	"Draw2D",
	"Draw3D",
	"FileDropped",
	"MouseButtonPressed",
	"MouseButtonDown",
	"MouseButtonUp",
};

int TriggerCallback(lua_State* L, const char* event)
{
	int status = 1;
	lua_getglobal(L, "EVENT");
	lua_getfield(L, -1, event);
	if (lua_istable(L, -3)) {
		lua_pushnil(L);
		while (lua_next(L, -4) != 0) {
			if (lua_isfunction(L, -1)) {
				status = lua_pcall(L, 0, 0, 0);
				if ( status != LUA_OK) {
PRINT(lua_tostring(L, -1))
				}
			}
		}
	}
	return status;
}

int MouseButtonPressedCallback(lua_State* L)
{
	return TriggerCallback(L, "MouseButtonPressed");
}

int MouseButtonDownCallback(lua_State* L)
{
	return TriggerCallback(L, "MouseButtonDown");
}

int MouseButtonUpCallback(lua_State* L)
{
	return TriggerCallback(L, "MouseButtonUp");
}

int Draw2DCallback(lua_State* L)
{
	return TriggerCallback(L, "Draw2D");
}

int Draw3DCallback(lua_State* L)
{
	return TriggerCallback(L, "Draw3D");
}

int NewGameCallback(lua_State* L)
{
	return TriggerCallback(L, "NewGame");
}

int FreezeCallback(lua_State* L)
{
	return TriggerCallback(L, "Freeze");
}

int StepCallback(lua_State* L)
{
	return TriggerCallback(L, "Step");
}

int FileDroppedCallback(lua_State* L, char* dropped_file)
{
PRINT(dropped_file)
	int status = 1;
	lua_getglobal(L, "EVENT");
	lua_getfield(L, -1, "FileDropped");
	if (lua_istable(L, -3)) {
		lua_pushnil(L);
		while (lua_next(L, -4) != 0) {
			if (lua_isfunction(L, -1)) {
				lua_pushstring(L, dropped_file);
				status = lua_pcall(L, 1, 0, 0);
				if (status != LUA_OK) {
PRINT(lua_tostring(L, -1))
				}
			}
		}
	}
	return status;
}

int NearCallback(lua_State* L, CollisionData collision)
{
	int status = 1;
	lua_getglobal(L, "EVENT");
	lua_getfield(L, -1, "NearCallback");
	if (lua_istable(L, -3)) {
		lua_pushnil(L);
		while (lua_next(L, -4) != 0) {
			if (lua_isfunction(L, -1)) {
				lua_newtable(L);
				for (int i = 1; 3 <= i; i += 1) {
					lua_pushnumber(L, collision.contacts.geom.pos[i]);
					lua_rawseti(L, -2, i);
				}
				status = lua_pcall(L, 1, 0, 0);
				if (status != LUA_OK) {
PRINT(lua_tostring(L, -1))
				}
			}
		}
	}
	return status;
}

int ConsoleCallback(lua_State* L, const char* message)
{
	int status = 1;
	lua_getglobal(L, "EVENT");
	lua_getfield(L, -1, "Console");
	if (lua_istable(L, -3)) {
		lua_pushnil(L);
		while (lua_next(L, -4) != 0) {
			if (lua_isfunction(L, -1)) {
				lua_pushstring(L, message);
				status = lua_pcall(L, 1, 0, 0);
				if (status != LUA_OK) {
PRINT(lua_tostring(L, -1))
				}
			}
		}
		lua_pop(L, 1);
	}
	return status;
}

// EVENT.SetCallback("Draw2D", "DS", function()  end)
int SetCallback(lua_State* L)
{
	const char* event = lua_tostring(L, -1);
	const char* handle = lua_tostring(L, -2);
	if (lua_isfunction(L, -3)) {
		lua_getglobal(L, "EVENT");
		lua_getfield(L, -4, event);
		if (lua_istable(L, -5)) {
			lua_gettable(L, -6);
			lua_setfield(L, -3, handle);
		}
	}
	return 1;
}

int GetCallback(lua_State* L)
{
	const char* event = lua_tostring(L, -1);
	const char* handle = lua_tostring(L, -2);
	return 1;
}

static const luaL_Reg api_callbacks[] {
	{"SetCallback", SetCallback},
	{"GetCallback", GetCallback},
	{NULL, NULL},
};

int luaopen_api_callbacks(lua_State* L) {
	luaL_register(L, "EVENT", api_callbacks);

	lua_getglobal(L, "EVENT");
	for (auto event : events) {
		lua_newtable(L);
		lua_setfield(L, -2, event);
	}
	lua_pop(L, 1);
	return 1;
}
