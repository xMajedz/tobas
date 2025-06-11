#include <luau.hpp>

int luau_do(lua_State* L, const char* string, const char* chunkname)
{
	Bytecode bytecode = { 0 };
	bytecode.data = luau_compile(string, strlen(string), NULL, &bytecode.size);
	int result = luau_load(L, chunkname, bytecode.data, bytecode.size, 0);
	int status = lua_pcall(L, 0, 0, 0);
	free(bytecode.data);
	return status;
}

int luau_dostring(lua_State* L, const char* string)
{
	return luau_do(L, string, "=dostring");
}

int luau_dostring(lua_State* L, const char* string, const char* chunkname)
{
	return luau_do(L, string, chunkname);
}

int luau_dofile(lua_State* L, const char* filename)
{
	char* text = LoadFileText(filename);
	int status = luau_dostring(L, text, "=dofile");
	UnloadFileText(text);
	return status;
}

int luau_dofile(lua_State* L, const char* filename, const char* chunkname)
{
	char* text = LoadFileText(filename);
	int status = luau_dostring(L, text, chunkname);
	UnloadFileText(text);
	return status;
}
