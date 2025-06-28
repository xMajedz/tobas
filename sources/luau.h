#include <cstdlib>
#include <cstring>

#include <lua.h>
#include <lualib.h>
#include <luacode.h>

struct Bytecode {
	char* data;
	size_t size;
};

int luau_do(lua_State* L, const char* string, const char* chunkname);

int luau_dostring(lua_State* L, const char* string);

int luau_dostring(lua_State* L, const char* string, const char* chunkname);

int luau_dofile(lua_State* L, const char* filename);

int luau_dofile(lua_State* L, const char* filename, const char* chunkname);

int luau_require(lua_State* L, const char* filename);
