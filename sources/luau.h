#pragma once
#include "lua.h"
#include "lualib.h"
#include "luacode.h"

#include <string_view>

namespace Luau
{
	int dostring (lua_State* L, std::string_view string);
	int dostring (lua_State* L, std::string_view string,   std::string_view chunkname);
	int dofile   (lua_State* L, std::string_view filename);
	int dofile   (lua_State* L, std::string_view filename, std::string_view chunkname);
	int require  (lua_State* L, std::string_view filename);

	static void (*log)(const char* msg) = nullptr;

	void setlogcallback(void(*callback)(const char*));
}
