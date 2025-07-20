#include "common.h"
#include "luau.h"

using namespace raylib;

struct Bytecode {
private:
	char* m_data;
	size_t m_size;
public:
	char* data()
	{
		return m_data;
	};

	size_t size()
	{
		return m_size;
	};

	Bytecode(std::string_view string)
	{
		m_data = luau_compile(string.data(), string.size(), NULL, &m_size);
	};

	~Bytecode()
	{
		delete m_data;
	};
};

static int run(lua_State* L, std::string_view string, std::string_view chunkname)
{
	Bytecode bytecode(string);
	int result = luau_load(L, TextFormat("=%s", chunkname.data()), bytecode.data(), bytecode.size(), 0);
	int nresults = 1;
	int status = lua_pcall(L, 0, nresults, 0);
	if (status != LUA_OK) { LOG(lua_tostring(L, -1)) }
	return nresults;
}

int Luau::dostring(lua_State* L, std::string_view string, std::string_view chunkname)
{
	return run(L, string, chunkname);
}

int Luau::dostring(lua_State* L, std::string_view string)
{
	return dostring(L, string, "dostring");
}

int Luau::dofile(lua_State* L, std::string_view filepath, std::string_view chunkname)
{
	const char* path = TextFormat("%s.luau", filepath.data());
	if (!FileExists(path)) return 1;
	char* text = LoadFileText(path);
	int status = dostring(L, text, chunkname);
	UnloadFileText(text);
	return status;
}

int Luau::dofile(lua_State* L, std::string_view filepath)
{
	return dofile(L, filepath, TextFormat("dofile:%s", filepath.data()));
}

int Luau::require(lua_State* L, std::string_view filename)
{
	const char* _requirepaths = "scripts/?.luau;scripts/?/?.luau";
	const char* requirepaths[] = {"scripts/%s", "scripts/%s/%s"};
	int status = 1;
	for (const auto& path : requirepaths) { 
		status = dofile(
				L,
				TextFormat(path, filename.data(), filename.data()),
				TextFormat("require:%s", filename.data())
		);
		if (status == LUA_OK) return status;
	}
	return status;
}
