#include "api.h"
#include "raylib.h"
#include "common.h"

static int RAYLIB_DrawText(lua_State* L)
{
	Color color;
	lua_rawgeti(L, -1, 1);
	color.r = lua_tonumber(L, -1);
	lua_rawgeti(L, -2, 2);
	color.g = lua_tonumber(L, -1);
	lua_rawgeti(L, -3, 3);
	color.b = lua_tonumber(L, -1);
	lua_rawgeti(L, -4, 4);
	color.a = lua_tonumber(L, -1);

	int fontSize = lua_tointeger(L, -6);
	int posY = lua_tointeger(L, -7);
	int posX = lua_tointeger(L, -8);
	const char* text = lua_tostring(L, -9); 	
	DrawText(text, posX, posY, fontSize, color);

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int RAYLIB_DrawRectangle(lua_State* L)
{
	Color color;
	lua_rawgeti(L, -1, 1);
	color.r = lua_tonumber(L, -1);
	lua_rawgeti(L, -2, 2);
	color.g = lua_tonumber(L, -1);
	lua_rawgeti(L, -3, 3);
	color.b = lua_tonumber(L, -1);
	lua_rawgeti(L, -4, 4);
	color.a = lua_tonumber(L, -1);

	int height = lua_tointeger(L, -6);
	int width = lua_tointeger(L, -7);
	int posY = lua_tointeger(L, -8);
	int posX = lua_tointeger(L, -9);
	DrawRectangle(posX, posY, width, height, color);

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int RAYLIB_DrawRectangleLines(lua_State* L)
{
	Color color;
	lua_rawgeti(L, -1, 1);
	color.r = lua_tonumber(L, -1);
	lua_rawgeti(L, -2, 2);
	color.g = lua_tonumber(L, -1);
	lua_rawgeti(L, -3, 3);
	color.b = lua_tonumber(L, -1);
	lua_rawgeti(L, -4, 4);
	color.a = lua_tonumber(L, -1);

	int height = lua_tointeger(L, -6);
	int width = lua_tointeger(L, -7);
	int posY = lua_tointeger(L, -8);
	int posX = lua_tointeger(L, -9);
	DrawRectangleLines(posX, posY, width, height, color);

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}
static int RAYLIB_LoadFileText(lua_State* L)
{
	const char* filename = lua_tostring(L, -1);
	char* text = LoadFileText(filename);
	lua_pushstring(L, text);
	UnloadFileText(text);
	return 1;
}

static int RAYLIB_LoadDirectoryFilesEx(lua_State* L)
{
	const char* path = lua_tostring(L, -1);
	const char* filter = lua_tostring(L, -2);
	bool scanSubdirs = lua_toboolean(L, -3);
	FilePathList list = LoadDirectoryFilesEx(path, filter, scanSubdirs == true);
	lua_newtable(L);
	for (int i = 0; i < list.count; ++i) {
		lua_pushstring(L, list.paths[i]);
		lua_rawseti(L, -2, i + 1);
	}
	UnloadDirectoryFiles(list);
	return 1;
}


static int RAYLIB_LoadDirectoryFiles(lua_State* L)
{
	const char* path = lua_tostring(L, -1);
	FilePathList list = LoadDirectoryFiles(path);
	lua_newtable(L);
	for (int i = 0; i < list.count; ++i) {
		lua_pushstring(L, list.paths[i]);
		lua_rawseti(L, -2, i + 1);
	}
	UnloadDirectoryFiles(list);
	return 1;
}

static int RAYLIB_GetMousePosition(lua_State* L)
{
	Vector2 MousePosition = GetMousePosition();
	lua_newtable(L);
	lua_pushnumber(L, MousePosition.x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, MousePosition.y);
	lua_setfield(L, -2, "y");
	return 1;
}

static int RAYLIB_GetTime(lua_State* L)
{
	lua_pushnumber(L, GetTime());
	return 1;
}

static int RAYLIB_GetFrameTime(lua_State* L)
{
	lua_pushnumber(L, GetFrameTime());
	return 1;
}

static int RAYLIB_GetFPS(lua_State* L)
{
	lua_pushnumber(L, GetFPS());
	return 1;
}

static const luaL_Reg api_raylib[] {
	{"DrawText", RAYLIB_DrawText},
	{"DrawRectangle", RAYLIB_DrawRectangle},
	{"DrawRectangleLines", RAYLIB_DrawRectangleLines},
	{"LoadFileText", RAYLIB_LoadFileText},
	{"LoadDirectoryFilesEx", RAYLIB_LoadDirectoryFilesEx},
	{"LoadDirectoryFiles", RAYLIB_LoadDirectoryFiles},
	{"GetMousePosition", RAYLIB_GetMousePosition},
	{"GetTime", RAYLIB_GetTime},
	{"GetFrameTime", RAYLIB_GetFrameTime},
	{"GetFPS", RAYLIB_GetFPS},
	{NULL, NULL},
};

int luaopen_api_raylib(lua_State* L)
{
	luaL_register(L, "RL", api_raylib);
	return 1;
}
