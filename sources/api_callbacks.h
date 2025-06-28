#include <luau.h>
#include <game_class.h>

int NewGameCallback(lua_State* L);

int FreezeCallback(lua_State* L);

int StepCallback(lua_State* L);

int NearCallback(lua_State* L, CollisionData collision);

int Draw2DCallback(lua_State* L);

int Draw3DCallback(lua_State* L);


int MouseButtonPressedCallback(lua_State* L);

int MouseButtonDownCallback(lua_State* L);

int MouseButtonUpCallback(lua_State* L);

int SetupCallbacks(lua_State* L);

int FileDroppedCallback(lua_State* L, char* dropped_file);

int ConsoleCallback(lua_State* L, const char* message);

int luaopen_api_callbacks(lua_State* L);
