#ifndef API_H
#define API_H
#include <luau.h>
#include <common.h>
#include <game.h>

struct API {
	Game* game;
	Player* player;
	Joint* joint;
	Body* body;
	Body* object;

	API();
	API(Game* game_ptr);
};

int API_turnframes(lua_State* L);

int API_numplayers(lua_State* L);

int API_friction(lua_State* L);

int API_engagedistance(lua_State* L);

int API_engageheight(lua_State* L);

int API_engagepos(lua_State* L);

int API_engagerot(lua_State* L);

int API_gravity(lua_State* L);

int API_mod(lua_State* L);

int API_object(lua_State* L);

int API_player(lua_State* L);

int API_body(lua_State* L);

int API_joint(lua_State* L);

int API_shape(lua_State* L);

int API_position(lua_State* L);

int API_orientation(lua_State* L);

int API_sides(lua_State* L);

int API_density(lua_State* L);

int API_static(lua_State* L);

int API_radius(lua_State* L);

int API_length(lua_State* L);

int API_strength(lua_State* L);

int API_strength_alt(lua_State* L);

int API_velocity(lua_State* L);

int API_velocity_alt(lua_State* L);

int API_axis(lua_State* L);

int API_axis_alt(lua_State* L);

int API_range(lua_State* L);

int API_range_alt(lua_State* L);

int API_connections(lua_State* L);

int API_connection_type(lua_State* L);

int API_dofile(lua_State* L);

int loadscript(lua_State* L, const char* scriptpath);

int API_loadscript(lua_State* L);

int loadmod(lua_State* L, const char* modpath);

int API_loadmod(lua_State* L);

int API_require(lua_State* L);

int API_GetWindowSize(lua_State* L);

int luaopen_api_main(lua_State* L);
#endif
