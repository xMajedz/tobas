#pragma once
#include "common.h"
#include "luau.h"
#include "game.h"

namespace API
{
	static lua_State* L;

	static lua_State* DrawT;
	static lua_State* UpdateT;

	static Gamerules rules;

	static std::vector<Body>   o_vector;
	static std::vector<Player> p_vector;
	 
	static size_t o_count;
	static size_t p_count;

	static Body*   o;
	static Player* p;
	
	static std::map<std::string_view, BodyID> b_map;
	static std::vector<Body>  b_vector;
	static std::vector<Joint> j_vector;

	static size_t b_count;
	static size_t j_count;
	
	static Body*  b;
	static Joint* j;
	
	void Init();
	void Reset();
	void Close();

	lua_State* GetL();
	Gamerules GetRules();
	std::vector<Body> GetObjects();
	std::vector<Player> GetPlayers();
	
	int TriggerCallback(const char* event, void* arg);
	int UpdateCallback(dReal dt);
	int DrawCallback();
	int NewGameCallback();
	int FreezeCallback();
	int StepCallback();

	int FileDroppedCallback(std::string_view dropped_file);
	int ConsoleCallback(const char* message);

	void SetCallback(const char* event, const char* handle, lua_CFunction function);
	lua_CFunction GetCallback(const char* event, const char* handle);

	int loadmod(std::string_view modpath);
	int loadscript(std::string_view scriptpath);
}

namespace Console
{
	static size_t message_count = 0;
	static size_t message_length = 1024;
	static const char* messages[1024];
	//static const char* last_message;
	static char last_message[1024];
	static bool has_message;

	static void (*m_callback)(const char*) = nullptr;

	void Update();

	void log(const char* message);
	void SetCallback(void (*callback)(const char*));
	void SetMessage(const char* message);
};


int luaopen_api_main(lua_State* L);

int luaopen_api_game(lua_State* L);

int luaopen_api_net(lua_State* L);

int luaopen_api_raylib(lua_State* L);

int luaopen_api_raygui(lua_State* L);

int luaopen_api_raymath(lua_State* L);
