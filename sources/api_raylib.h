#include <luau.h>
#include <common.h>
#include <raylib.h>

int api_DrawText(lua_State* L);

int api_DrawRectangle(lua_State* L);

int api_DrawRectangleLines(lua_State* L);

int api_LoadFileText(lua_State* L);

int api_LoadDirectoryFilesEx(lua_State* L);

int api_LoadDirectoryFiles(lua_State* L);

int api_GetMousePosition(lua_State* L);

int api_GetTime(lua_State* L);

int api_GetFrameTime(lua_State* L);

int api_GetFPS(lua_State* L);

int luaopen_api_raylib(lua_State* L);

static const luaL_Reg api_raylib[] {
	{"DrawText", api_DrawText},
	{"DrawRectangle", api_DrawRectangle},
	{"DrawRectangleLines", api_DrawRectangleLines},
	{"LoadFileText", api_LoadFileText},
	{"LoadDirectoryFilesEx", api_LoadDirectoryFilesEx},
	{"LoadDirectoryFiles", api_LoadDirectoryFiles},
	{"GetMousePosition", api_GetMousePosition},
	{"GetTime", api_GetTime},
	{"GetFrameTime", api_GetFrameTime},
	{"GetFPS", api_GetFPS},
	{NULL, NULL},
};
