#include <luau.h>
#include <raylib.h>
#include <iostream>
#define PRINT(X) std::cout << "-- TEST " << X << " --" << std::endl;
#define PRINT_A PRINT("A")
#define PRINT_B PRINT("B")
#define PRINT_C PRINT("C")
#define LUAU_FREE(Data) free(Data)

int luau_do(lua_State* L, const char* string, const char* chunkname)
{
	Bytecode bytecode = {0};
	bytecode.data = luau_compile(string, strlen(string), NULL, &bytecode.size);
	int result = luau_load(L, TextFormat("=%s", chunkname), bytecode.data, bytecode.size, 0);
	int nresult = 1;
	int status = lua_pcall(L, 0, nresult, 0);
	if (status != LUA_OK) {
PRINT(lua_tostring(L, -1))
		return status;
	}
	LUAU_FREE(bytecode.data);
	return nresult;
}

int luau_dostring(lua_State* L, const char* string, const char* chunkname)
{
	return luau_do(L, string, chunkname);
}

int luau_dostring(lua_State* L, const char* string)
{
	return luau_dostring(L, string, "dostring");
}

int luau_dofile(lua_State* L, const char* filepath, const char* chunkname)
{
	const char* path = TextFormat("%s.luau", filepath);
	if (!FileExists(path)) return 1;
	char* text = LoadFileText(path);
	int status = luau_dostring(L, text, chunkname);
	UnloadFileText(text);
	return status;
}

int luau_dofile(lua_State* L, const char* filepath)
{
	return luau_dofile(L, filepath, TextFormat("dofile:%s", filepath));
}

int luau_require(lua_State* L, const char* filename)
{
	//scripts/?.luau;scripts/?/?.luau
	const char* requirepaths[] = {"scripts/%s", "scripts/%s/%s"};
	int status = 1;
	for (auto& path : requirepaths) { 
		status = luau_dofile(
				L,
				TextFormat(path, filename, filename),
				TextFormat("require:%s", filename)
		);
		if (status == LUA_OK) return status;
	}
	return status;
}
