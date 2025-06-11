#ifndef API_H
#define API_H

enum GameContext {
	NoContext,
	ObjectContext,
	PlayerContext,
	BodyContext,
	JointContext
} DataContext = NoContext;

std::string object_key;
std::string player_key;
std::string body_key;
std::string joint_key;

int API_turnframes(lua_State* L);

int API_numplayers(lua_State* L);

int API_friction(lua_State* L);

int API_engagedistance(lua_State* L);

int API_engageheight(lua_State* L);

int API_engagepos(lua_State* L);

int API_engagerot(lua_State* L);

int API_gravity(lua_State* L);

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

int api_dofile(lua_State* L);

void api_set(lua_State* L, lua_CFunction fn ,const char* fn_name);

int luaopen_api_main(lua_State* L);

static const luaL_Reg api_main[] {
	{"turnframes", API_turnframes},
	{"numplayers", API_numplayers},
	{"friction", API_friction},
	{"engagedistance", API_engagedistance},
	{"engageheight", API_engageheight},
	{"engagepos", API_engagepos},
	{"engagerot", API_engagerot},
	{"gravity", API_gravity},
	{"object", API_object},
	{"player", API_player},
	{"body", API_body},
	{"joint", API_joint},
	{"shape", API_shape},
	{"position", API_position},
	{"orientation", API_orientation},
	{"sides", API_sides},
	{"density", API_density},
	{"static", API_static},
	{"radius", API_radius},
	{"length", API_length},
	{"strength", API_strength},
	{"strength_alt", API_strength_alt},
	{"velocity", API_velocity},
	{"velocity_alt", API_velocity_alt},
	{"axis", API_axis},
	{"axis_alt", API_axis_alt},
	{"range", API_range},
	{"range_alt", API_range_alt},
	{"connections", API_connections},
	{"connection_type", API_connection_type},
	{NULL, NULL},
};
#endif
