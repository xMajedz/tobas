#include "api.h"

using namespace raylib;
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

static int RAYGUI_GuiSetStyle(lua_State* L)
{
	int value = lua_tointeger(L, -1);
	int property = lua_tointeger(L, -2);
	int control = lua_tointeger(L, -3);
	GuiSetStyle(control, property, value);
	lua_pushinteger(L, 1);
	return 1;
}

static int RAYGUI_GuiGetStyle(lua_State* L)
{
	int property = lua_tointeger(L, -1);
	int control = lua_tointeger(L, -2);
	int status = GuiGetStyle(control, property);
	lua_pushinteger(L, status);
	return 1;
}

static int RAYGUI_GuiWindowBox(lua_State* L)
{
	const char* title = lua_tostring(L, -1);
	float h = lua_tonumber(L, -2);
	float w = lua_tonumber(L, -3);
	float y = lua_tonumber(L, -4);
	float x = lua_tonumber(L, -5);

	int status = GuiWindowBox((Rectangle){x, y, w, h}, title);
	lua_pushboolean(L, (bool)status);
	return 1;
}

static int RAYGUI_GuiGroupBox(lua_State* L)
{
	const char* text = lua_tostring(L, -1);
	float h = lua_tonumber(L, -2);
	float w = lua_tonumber(L, -3);
	float y = lua_tonumber(L, -4);
	float x = lua_tonumber(L, -5);

	int status = GuiGroupBox((Rectangle){x, y, w, h}, text);
	lua_pushboolean(L, (bool)status);
	return 1;
}

static int RAYGUI_GuiLabel(lua_State* L)
{
	const char*  text = lua_tostring(L, -1);
	float h = lua_tonumber(L, -2);
	float w = lua_tonumber(L, -3);
	float y = lua_tonumber(L, -4);
	float x = lua_tonumber(L, -5);

	int status = GuiLabel((Rectangle){x, y, w, h}, text);
	lua_pushboolean(L, (bool)status);
	return 1;
}

static int RAYGUI_GuiButton(lua_State* L)
{
	const char*  text = lua_tostring(L, -1);
	float h = lua_tonumber(L, -2);
	float w = lua_tonumber(L, -3);
	float y = lua_tonumber(L, -4);
	float x = lua_tonumber(L, -5);

	int status = GuiButton((Rectangle){x, y, w, h}, text);
	lua_pushboolean(L, (bool)status);
	return 1;
}

static int RAYGUI_GuiMessageBox(lua_State* L)
{
	const char* buttons = lua_tostring(L, -1);
	const char* message = lua_tostring(L, -2);
	const char* title = lua_tostring(L, -3);
	float h = lua_tonumber(L, -4);
	float w = lua_tonumber(L, -5);
	float y = lua_tonumber(L, -6);
	float x = lua_tonumber(L, -7);

	int status = GuiMessageBox((Rectangle){x, y, w, h}, title, message, buttons);

	if (status == -1) {
		lua_pushnil(L);
		return 1;
	} else if (status == 0) {
		lua_pushboolean(L, false);
		return 1;
	} else if (status == 1) {
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int RAYGUI_GuiTextBoxTextSize = 1024;
static char RAYGUI_GuiTextBoxContent[1024];

static int RAYGUI_GuiTextBox(lua_State* L)
{
	lua_getglobal(L, "RAYGUI");
	lua_getfield(L, -1, "_VAR");
	lua_getfield(L, -1, "GuiTextBox");
	
	const char* _VAR = lua_tostring(L, -1);
	TextCopy(RAYGUI_GuiTextBoxContent, _VAR);

	bool editMode = lua_toboolean(L, -4);
	float h = lua_tonumber(L, -5);
	float w = lua_tonumber(L, -6);
	float y = lua_tonumber(L, -7);
	float x = lua_tonumber(L, -8);

	int status = GuiTextBox((Rectangle){x, y, w, h},
			RAYGUI_GuiTextBoxContent,
			RAYGUI_GuiTextBoxTextSize,
			editMode
	);

	lua_pushboolean(L, (bool)status);

	Luau::dostring(L, TextFormat(
			"_G[\"RAYGUI\"][\"_VAR\"][\"GuiTextBox\"]=\"%s\"", RAYGUI_GuiTextBoxContent)
	);

	return 1;
}

static int RAYGUI_GuiTextInputBox(lua_State* L)
{
	bool secretViewActive = lua_toboolean(L, -1);
	auto textMaxSize = lua_tointeger(L, -2);

	//char* text;

	auto buttons = lua_tostring(L, -3);
	auto message = lua_tostring(L, -4);
	auto title = lua_tostring(L, -5);
	float h = lua_tonumber(L, -6);
	float w = lua_tonumber(L, -7);
	float y = lua_tonumber(L, -8);
	float x = lua_tonumber(L, -9);

	int status = GuiTextInputBox(
		(Rectangle){x, y, w, h},
		title,
		message,
		buttons,
		RAYGUI_GuiTextBoxContent,
		textMaxSize,
		&secretViewActive
	);

	return 1;
}

static float RAYGUI_GuiSliderVal = 1.00;

static int RAYGUI_GuiSlider(lua_State* L)
{
	lua_getglobal(L, "RAYGUI");
	lua_getfield(L, -1, "_VAR");
	lua_getfield(L, -1, "GuiSlider");

	RAYGUI_GuiSliderVal = lua_tonumber(L, -1);

	float max = lua_tonumber(L, -4);
	float min = lua_tonumber(L, -5);

	auto textL = lua_tostring(L, -6);
	auto textR = lua_tostring(L, -7);

	float h = lua_tonumber(L, -8);
	float w = lua_tonumber(L, -9);
	float y = lua_tonumber(L, -10);
	float x = lua_tonumber(L, -11);

	int status  = GuiSlider(
		(Rectangle){x, y, w, h},
		textR,
		textL,
		&RAYGUI_GuiSliderVal,
		min,
		max
	);

	lua_pushboolean(L, status);

	return 1;
}

static float RAYGUI_GuiSliderBarVal = 1.00;

static int RAYGUI_GuiSliderBar(lua_State* L)
{
	lua_getglobal(L, "RAYGUI");
	lua_getfield(L, -1, "_VAR");
	lua_getfield(L, -1, "GuiSliderBar");

	RAYGUI_GuiSliderBarVal = lua_tonumber(L, -1);

	float max = lua_tonumber(L, -4);
	float min = lua_tonumber(L, -5);

	auto textL = lua_tostring(L, -6);
	auto textR = lua_tostring(L, -7);

	float h = lua_tonumber(L, -8);
	float w = lua_tonumber(L, -9);
	float y = lua_tonumber(L, -10);
	float x = lua_tonumber(L, -11);

	int status  = GuiSliderBar(
		(Rectangle){x, y, w, h},
		textR,
		textL,
		&RAYGUI_GuiSliderBarVal,
		min,
		max
	);

	lua_pushboolean(L, status);

	return 1;
}

static const luaL_Reg api_raygui[] {
	{"GuiSetStyle", RAYGUI_GuiSetStyle},
	{"GuiGetStyle", RAYGUI_GuiGetStyle},

	{"GuiWindowBox", RAYGUI_GuiWindowBox},
	{"GuiGroupBox", RAYGUI_GuiGroupBox},
	{"GuiMessageBox", RAYGUI_GuiMessageBox},

	{"GuiLabel", RAYGUI_GuiLabel},
	{"GuiButton", RAYGUI_GuiButton},

	{"GuiTextBox", RAYGUI_GuiTextBox},
	{"GuiTextInputBox", RAYGUI_GuiTextInputBox},

	{"GuiSlider", RAYGUI_GuiSlider},
	{"GuiSliderBar", RAYGUI_GuiSliderBar},

	{NULL, NULL},
};

int luaopen_api_raygui(lua_State* L)
{
	luaL_register(L, "RAYGUI", api_raygui);

	lua_getglobal(L, "RAYGUI");
	lua_pushnumber(L, RAYGUI_VERSION_MAJOR);
	lua_setfield(L, -2, "VERSION_MAJOR");
	lua_pushnumber(L, RAYGUI_VERSION_MINOR);
	lua_setfield(L, -2, "VERSION_MINOR");
	lua_pushnumber(L, RAYGUI_VERSION_PATCH);
	lua_setfield(L, -2, "VERSION_PATCH");
	lua_pushstring(L, RAYGUI_VERSION);
	lua_setfield(L, -2, "VERSION");
	lua_newtable(L);

	lua_pushstring(L, "");
	lua_setfield(L, -2, "GuiTextBox");

	lua_setfield(L, -2, "_VAR");
	lua_pop(L, 1);
	return 1;
}
