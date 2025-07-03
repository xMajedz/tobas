#include "api.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

static int RAYGUI_GuiButton(lua_State* L)
{
	auto text = lua_tostring(L, -1);
	float h = lua_tonumber(L, -2);
	float w = lua_tonumber(L, -3);
	float y = lua_tonumber(L, -4);
	float x = lua_tonumber(L, -5);

	int status = GuiButton((Rectangle){x, y, w, h}, text);
	lua_pushinteger(L, status);
	return 1;
}

static int RAYGUI_GuiMessageBox(lua_State* L)
{
	auto buttons = lua_tostring(L, -1);
	auto message = lua_tostring(L, -2);
	auto title = lua_tostring(L, -3);
	float h = lua_tonumber(L, -4);
	float w = lua_tonumber(L, -5);
	float y = lua_tonumber(L, -6);
	float x = lua_tonumber(L, -7);

	int status = GuiMessageBox((Rectangle){x, y, w, h}, title, message, buttons);
	lua_pushinteger(L, status);
	return 1;
}

//to_cfunction

static const luaL_Reg api_raygui[] {
	{"GuiButton", RAYGUI_GuiButton},
	{"GuiMessageBox", RAYGUI_GuiMessageBox},
	{NULL, NULL},
};

int luaopen_api_raygui(lua_State* L)
{
	luaL_register(L, "RAYGUI", api_raygui);
	return 1;
}
