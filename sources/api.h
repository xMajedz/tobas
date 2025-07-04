#pragma once
#include "common.h"
#include "luau.h"
#include "game.h"

namespace API
{
	static lua_State* L;

	static Gamerules rules;

	static std::vector<Body> objects;
	static std::vector<Player> players;
	 
	static size_t o_count;
	static size_t p_count;

	static Body* object;
	static Player* player;
	
	static std::vector<Body> b_vector;
	static std::vector<Joint> j_vector;

	static size_t b_count;
	static size_t j_count;

	static Body* body;
	static Joint* joint;
	
	void Init();
	void Reset();
	void Close();

	lua_State* GetL();
	Gamerules GetRules();
	array<Body> GetObjects();
	array<Player> GetPlayers();
	array<Body> GetBody();
	array<Joint> GetJoint();
	
	int TriggerCallback(const char* event);
	int TriggerCallback(const char* event, const char* str);
	int TriggerCallback(const char* event, dReal dt);

	int MouseButtonPressedCallback();
	int MouseButtonDownCallback();
	int MouseButtonUpCallback();
	int UpdateCallback(dReal dt);
	int DrawCallback();
	int Draw2DCallback();
	int Draw3DCallback();
	int NewGameCallback();
	int FreezeCallback();
	int StepCallback();

	int NearCallback(CollisionData s_collision);
	int FileDroppedCallback(const char* dropped_file);
	int ConsoleCallback(const char* message);

	void SetCallback(const char* event, const char* handle, lua_CFunction function);
	lua_CFunction GetCallback(const char* event, const char* handle);

	int loadmod(const char* modpath);
	int loadscript(const char* scriptpath);
}

namespace Console {
	static const char* messages[1024];
	static const char* s_last_message;
	static bool s_has_event;

	void log(const char* message);
	void SetMessage(const char* message);
	const char* GetMessage();
	void ResetHasEvent();
	void SetHasEvent();
	bool GetHasEvent();
};


int luaopen_api_main(lua_State* L);

int luaopen_api_game(lua_State* L);

int luaopen_api_raylib(lua_State* L);

int luaopen_api_raygui(lua_State* L);

int luaopen_api_raymath(lua_State* L);
