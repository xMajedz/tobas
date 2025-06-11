#include <callbacks.hpp>

void MouseButtonPressedCallback(lua_State* L)
{
	lua_getglobal(L, "MouseButtonPressed");
	if (lua_istable(L, -1)) {
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (lua_isfunction(L, -1)) {
				if (lua_pcall(L, 0, 0, 0) == LUA_OK) {
				}
			}
		}
		lua_pop(L, 1);
	}
}

void MouseButtonDownCallback(lua_State* L)
{
	lua_getglobal(L, "MouseButtonDown");
	if (lua_istable(L, -1)) {
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (lua_isfunction(L, -1)) {
				if (lua_pcall(L, 0, 0, 0) == LUA_OK) {
				}
			}
		}
		lua_pop(L, 1);
	}
}

void MouseButtonUpCallback(lua_State* L)
{
	lua_getglobal(L, "MouseButtonUp");
	if (lua_istable(L, -1)) {
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (lua_isfunction(L, -1)) {
				if (lua_pcall(L, 0, 0, 0) == LUA_OK) {
				}
			}
		}
		lua_pop(L, 1);
	}
}

void FileDroppedCallback(lua_State* L, char* dropped_file)
{
	lua_getglobal(L, "FileDropped");
	if (lua_istable(L, -1)) {
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (lua_isfunction(L, -1)) {
				lua_pushstring(L, dropped_file);
				if (lua_pcall(L, 1, 0, 0) == LUA_OK) {
				}
			}
		}
		lua_pop(L, 1);
	}
}

void Draw2DCallback(lua_State* L)
{
	lua_getglobal(L, "Draw2D");
	if (lua_istable(L, -1)) {
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (lua_isfunction(L, -1)) {
				if (lua_pcall(L, 0, 0, 0) == LUA_OK) {
				}
			}
		}
		lua_pop(L, 1);
	}
}

void Draw3DCallback(lua_State* L)
{
	lua_getglobal(L, "Draw3D");
	if (lua_istable(L, -1)) {
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (lua_isfunction(L, -1)) {
				if (lua_pcall(L, 0, 0, 0) == LUA_OK) {
				}
			}
		}
		lua_pop(L, 1);
	}
}

void SetupCallbacks(lua_State* L)
{
	lua_newtable(L);
	lua_setglobal(L, "Draw2D");
	lua_newtable(L);
	lua_setglobal(L, "Draw3D");
	lua_newtable(L);
	lua_setglobal(L, "FileDropped");
	lua_newtable(L);
	lua_setglobal(L, "MouseButtonPressed");
	lua_newtable(L);
	lua_setglobal(L, "MouseButtonDown");
	lua_newtable(L);
	lua_setglobal(L, "MouseButtonUp");
}
