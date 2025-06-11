#ifndef CALLBACKS_H
#define CALLBACKS_H

void NewGameCallback(lua_State* L);

void FreezeCallback(lua_State* L);

void StepCallback(lua_State* L);

void Draw2DCallback(lua_State* L);

void Draw3DCallback(lua_State* L);

void FileDroppedCallback(lua_State* L, char* dropped_file);

void MouseButtonPressedCallback(lua_State* L);

void MouseButtonDownCallback(lua_State* L);

void MouseButtonUpCallback(lua_State* L);

void SetupCallbacks(lua_State* L);
#endif
