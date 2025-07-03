#include "api.h"

extern Window window;
extern Console console;

enum GameContext {
	NoContext,
	ObjectContext,
	PlayerContext,
	BodyContext,
	JointContext,
} DataContext = NoContext;

void API::Init()
{
	L = luaL_newstate();
	luaopen_api_main(L);
	luaopen_api_game(L);
	luaopen_api_raylib(L);
	luaopen_api_raygui(L);
	luaopen_api_raymath(L);
	luaL_openlibs(L);
	luaL_sandbox(L);

	loadscript("init");

	player->body = GetBody();
	b_vector.clear();
	b_count = 0;
	
	player->joint = GetJoint();
	j_vector.clear();
	j_count = 0;
}

void API::Reset()
{
	rules = {.mod = ""};

	objects.clear();
	players.clear();
	
	object = 0;
	player = 0;

	o_count = 0;
	p_count = 0;

	b_vector.clear();
	j_vector.clear();

	body = 0;
	joint = 0;

	b_count = 0;
	j_count = 0;
}

void API::Close()
{
	lua_close(L);
}

lua_State* API::GetL()
{
	return L;
}

Gamerules API::GetRules()
{
	return rules;
}

array<Body> API::GetObjects()
{
	auto o_array = new Body[o_count];
	for (int i = 0; i < o_count; i += 1) {
		o_array[i] = objects[i];
	}
	return (array<Body>){o_array, o_count};
}

array<Player> API::GetPlayers()
{
	auto p_array = new Player[p_count];
	for (int i = 0; i < p_count; i += 1) {
		p_array[i] = players[i];
	}
	return (array<Player>){p_array, p_count};
}

array<Body> API::GetBody()
{
	auto b_array = new Body[b_count];
	for (int i = 0; i < b_count; i += 1) {
		b_array[i] = b_vector[i];
	}
	return (array<Body>){b_array, b_count};
}

array<Joint> API::GetJoint()
{
	auto j_array = new Joint[j_count];
	for (int i = 0; i < j_count; i += 1) {
		j_array[i] = j_vector[i];
	}
	return (array<Joint>){j_array, j_count};
}

int API::TriggerCallback(const char* event)
{
	int status = 1;
	lua_getglobal(L, "API");
	lua_getfield(L, -1, event);
	if (lua_istable(L, -3)) {
		lua_pushnil(L);
		while (lua_next(L, -4) != 0) {
			if (lua_isfunction(L, -1)) {
				status = lua_pcall(L, 0, 0, 0);
				if ( status != LUA_OK) {
LOG(lua_tostring(L, -1))
				}
			}
		}
	}
	return status;
}

int API::TriggerCallback(const char* event, dReal dt)
{
	return TriggerCallback(event);
}

int API::MouseButtonPressedCallback()
{
	return TriggerCallback("MouseButtonPressed");
}

int API::MouseButtonDownCallback()
{
	return TriggerCallback("MouseButtonDown");
}

int API::MouseButtonUpCallback()
{
	return TriggerCallback("MouseButtonUp");
}

int API::UpdateCallback(dReal dt)
{
	return TriggerCallback("Update", dt);
}

int API::DrawCallback()
{
	return TriggerCallback("Draw");
}

int API::Draw2DCallback()
{
	return TriggerCallback("Draw2D");
}

int API::Draw3DCallback()
{
	return TriggerCallback("Draw3D");
}

int API::NewGameCallback()
{
	return TriggerCallback("NewGame");
}

int API::FreezeCallback()
{
	return TriggerCallback("Freeze");
}

int API::StepCallback()
{
	return TriggerCallback("Step");
}

int API::NearCallback(CollisionData collision)
{
	int status = 1;
	lua_getglobal(L, "API");
	lua_getfield(L, -1, "NearCallback");
	if (lua_istable(L, -3)) {
		lua_pushnil(L);
		while (lua_next(L, -4) != 0) {
			if (lua_isfunction(L, -1)) {
				lua_newtable(L);
				for (int i = 1; 3 <= i; i += 1) {
					lua_pushnumber(L, collision.contacts.geom.pos[i]);
					lua_rawseti(L, -2, i);
				}
				status = lua_pcall(L, 1, 0, 0);
				if (status != LUA_OK) {
LOG(lua_tostring(L, -1))
				}
			}
		}
	}
	return status;
}

int API::FileDroppedCallback(const char* dropped_file)
{
LOG(dropped_file)
	int status = 1;
	lua_getglobal(L, "EVENT");
	lua_getfield(L, -1, "FileDropped");
	if (lua_istable(L, -3)) {
		lua_pushnil(L);
		while (lua_next(L, -4) != 0) {
			if (lua_isfunction(L, -1)) {
				lua_pushstring(L, dropped_file);
				status = lua_pcall(L, 1, 0, 0);
				if (status != LUA_OK) {
LOG(lua_tostring(L, -1))
				}
			}
		}
	}
	return status;
}

int API::ConsoleCallback(const char* message)
{
	int status = 1;
	lua_getglobal(L, "EVENT");
	lua_getfield(L, -1, "Console");
	if (lua_istable(L, -3)) {
		lua_pushnil(L);
		while (lua_next(L, -4) != 0) {
			if (lua_isfunction(L, -1)) {
				lua_pushstring(L, message);
				status = lua_pcall(L, 1, 0, 0);
				if (status != LUA_OK) {
LOG(lua_tostring(L, -1))
				}
			}
		}
		lua_pop(L, 1);
	}
	return status;
}

int API::SetCallback(const char* event, const char* handle, lua_CFunction* calllback)
{
	if (lua_isfunction(L, -3)) {
		lua_getglobal(L, "API");
		lua_getfield(L, -4, event);
		if (lua_istable(L, -5)) {
			lua_gettable(L, -6);
			lua_setfield(L, -3, handle);
		}
	}
	return 1;
}

int API::GetCallback(const char* event, const char* handle)
{
	return 1;
}

int API::loadmod (const char* modpath)
{
	return luau_dofile(
		L,
		TextFormat("mods/%s", modpath),
		TextFormat("%s:%s", "loadmod", modpath)
	);
}

int API::loadscript (const char* scriptpath)
{
	const char* path = TextFormat("scripts/%s", scriptpath);
	return luau_dofile(
		L,
		path,
		TextFormat("%s:%s", "loadscript", scriptpath)
	);
}

static int API_SetCallback(lua_State* L)
{
	const char* event = lua_tostring(L, -1);
	const char* handle = lua_tostring(L, -2);
	return API::SetCallback(event, handle, 0);
}

static int API_GetCallback(lua_State* L)
{
	const char* event = lua_tostring(L, -1);
	const char* handle = lua_tostring(L, -2);
	API::GetCallback(event, handle);
	return 1;
}

static int API_turnframes(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number turnframes = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			API::rules.turnframes = turnframes;
		} break;
		case ObjectContext: {
			// Error Handling
		} break;
		case BodyContext: {
			// Error Handling
		} break;
		case JointContext: {
			// Error Handling
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_numplayers(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number numplayers = lua_tonumber(L, -1); 

	switch(DataContext) {
		case NoContext: {
			API::rules.numplayers = numplayers;
		} break;
		case ObjectContext: {
			// Error Handling
		} break;
		case BodyContext: {
			// Error Handling
		} break;
		case JointContext: {
			// Error Handling
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_friction(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number friction = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			API::rules.friction = friction;
		} break;
		case ObjectContext: {
			// Error Handling
		} break;
		case BodyContext: {
			// Error Handling
		} break;
		case JointContext: {
			// Error Handling
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_engagedistance(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number distance = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			API::rules.engagedistance = distance;
		} break;
		case ObjectContext: {
			// Error Handling
		} break;
		case BodyContext: {
			// Error Handling
		} break;
		case JointContext: {
			// Error Handling
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_engageheight(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number height = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			API::rules.engageheight = height;
		} break;
		case ObjectContext: {
			// Error Handling
		} break;
		case BodyContext: {
			// Error Handling
		} break;
		case JointContext: {
			// Error Handling
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_engagepos(lua_State* L)
{
	Vector3 pos;
	lua_rawgeti(L, -1, 1);
	pos.x = lua_tonumber(L, -1);
	lua_rawgeti(L, -2, 2);
	pos.y = lua_tonumber(L, -1);
	lua_rawgeti(L, -3, 3);
	pos.z = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case PlayerContext: {
			API::player->use_engagepos = true;
			API::player->engagepos = pos;
		} break;
		case ObjectContext: {
			// Error Handling
		} break;
		case BodyContext: {
			// Error Handling
		} break;
		case JointContext: {
			// Error Handling
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_engagerot(lua_State* L)
{
	Vector3 rot;

	lua_rawgeti(L, -1, 1);
	rot.x = lua_tonumber(L, -1);
	lua_rawgeti(L, -2, 2);
	rot.y = lua_tonumber(L, -1);
	lua_rawgeti(L, -3, 3);
	rot.z = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case PlayerContext: {
			API::player->use_engagerot = true;
			API::player->engagerot = rot;
		} break;
		case ObjectContext: {
			// Error Handling
		} break;
		case BodyContext: {
			// Error Handling
		} break;
		case JointContext: {
			// Error Handling
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_gravity(lua_State* L)
{
	Vector3 gravity;
	lua_rawgeti(L, -1, 1);
	gravity.x = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	gravity.y = lua_tonumber(L, -1); 
	lua_rawgeti(L, -3, 3);
	gravity.z = lua_tonumber(L, -1); 

	switch(DataContext) {
		case NoContext: {
			API::rules.gravity = gravity;
		} break;
		case ObjectContext: {
			// Error Handling	
		} break;
		case BodyContext: {
			// Error Handling	
		} break;
		case JointContext: {
			// Error Handling	
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_mod(lua_State* L)
{
	API::rules.mod = lua_tostring(L, -1);

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_object(lua_State* L)
{
	DataContext = ObjectContext;
	Body o(lua_tostring(L, -1));
	API::objects.push_back(o);
	API::object = &API::objects[API::o_count];
	API::o_count += 1;

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_player(lua_State* L)
{
	DataContext = PlayerContext;
	const char* name = lua_tostring(L, -1);
	lua_Number result = 0;

	if (API::p_count < API::rules.numplayers) {
		if (0 != API::p_count) {
			API::player->body = API::GetBody();
			API::b_vector.clear();
			API::b_count = 0;

			API::player->joint = API::GetJoint();
			API::j_vector.clear();
			API::j_count = 0;
		}

		Player p(name);
		API::players.push_back(p);
		API::player = &API::players[API::p_count];
		API::p_count += 1;
		result = 1;
	}

	lua_pushnumber(L, result);
	return 1;
}

static int API_body(lua_State* L)
{
	DataContext = BodyContext;
	Body b(lua_tostring(L, -1));
	API::b_vector.push_back(b);
	API::body = &API::b_vector[API::b_count];
	API::b_count += 1;

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_joint(lua_State* L)
{
	DataContext = JointContext;
	Joint j(lua_tostring(L, -1));
	API::j_vector.push_back(j);
	API::joint = &API::j_vector[API::j_count];
	API::j_count += 1;

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_shape(lua_State* L)
{
	std::string shape = lua_tostring(L, -1);

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			if ("box" == shape) {
				API::object->shape = Box;
			} else if ("sphere" == shape) {
				API::object->shape = Sphere;
			} else if ("capsule" == shape) {
				API::object->shape = Capsule;
			} else if ("cylinder" == shape) {
				API::object->shape = Cylinder;
			} else  {
				//Error Handling
			}
		} break;
		case BodyContext: {
			if ("box" == shape) {
				API::body->shape = Box;
			} else if ("sphere" == shape) {
				API::body->shape = Sphere;
			} else if ("capsule" == shape) {
				API::body->shape = Capsule;
			} else if ("cylinder" == shape) {
				API::body->shape = Cylinder;
			} else  {
				//Error Handling
			}
		} break;
		case JointContext: {
			if ("box"  == shape) {
				API::joint->shape = Box;
			} else if ("sphere" == shape) {
				API::joint->shape = Sphere;
			} else if ("capsule" == shape) {
				API::joint->shape = Capsule;
			} else if ("cylinder" == shape) {
				API::joint->shape = Cylinder;
			} else  {
				//Error Handling
			}
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_position(lua_State* L)
{
	Vector3 position;
	lua_rawgeti(L, -1, 1);
	position.x = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	position.y = lua_tonumber(L, -1); 
	lua_rawgeti(L, -3, 3);
	position.z = lua_tonumber(L, -1); 
	
	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			API::object->position = position;
			API::object->freeze.position = position;
		} break;
		case BodyContext: {
			API::body->position = position;
			API::body->freeze.position = position;
		} break;
		case JointContext: {
			API::joint->position = position;
			API::joint->freeze.position = position;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_orientation(lua_State* L)
{
	Vector4 orientation;
	lua_rawgeti(L, -1, 1);
	orientation.w = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	orientation.x = lua_tonumber(L, -1); 
	lua_rawgeti(L, -3, 3);
	orientation.y = lua_tonumber(L, -1); 
	lua_rawgeti(L, -4, 4);
	orientation.z = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			API::object->orientation = orientation;
			API::object->freeze.orientation = orientation;

		} break;
		case BodyContext: {
			API::body->orientation = orientation;
			API::body->freeze.orientation = orientation;
		} break;
		case JointContext: {
			API::joint->orientation = orientation;
			API::joint->freeze.orientation = orientation;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_sides(lua_State* L)
{
	Vector3 sides;
	lua_rawgeti(L, -1, 1);
	sides.x = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	sides.y = lua_tonumber(L, -1); 
	lua_rawgeti(L, -3, 3);
	sides.z = lua_tonumber(L, -1); 

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			API::object->sides = sides;
		} break;
		case BodyContext: {
			API::body->sides = sides;
		} break;
		case JointContext: {
			API::joint->sides = sides;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_density(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number density = lua_tonumber(L, -1); 

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			API::object->density = density;
		} break;
		case BodyContext: {
			API::body->density = density;
		} break;
		case JointContext: {
			API::joint->density = density;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_static(lua_State* L)
{
	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			API::object->static_state = true;
		} break;
		case BodyContext: {
			API::body->static_state = true;
		} break;
		case JointContext: {
			API::joint->static_state = true;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_radius(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number radius = lua_tonumber(L, -1); 

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			API::object->radius = radius;
		} break;
		case BodyContext: {
			API::body->radius = radius;
		} break;
		case JointContext: {
			API::joint->radius = radius;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_length(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number length = lua_tonumber(L, -1); 

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			API::object->length = length;
		} break;
		case BodyContext: {
			API::body->length = length;
		} break;
		case JointContext: {
			API::joint->length = length;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_strength(lua_State* L)
{
	lua_Number strength;
	lua_rawgeti(L, -1, 1);
	strength = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			API::joint->strength = strength;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_strength_alt(lua_State* L)
{
	lua_Number strength_alt;
	lua_rawgeti(L, -1, 1);
	strength_alt = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			API::joint->strength_alt = strength_alt;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}


static int API_velocity(lua_State* L)
{
	lua_Number velocity;
	lua_rawgeti(L, -1, 1);
	velocity = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			API::joint->velocity = velocity;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_velocity_alt(lua_State* L)
{
	lua_Number velocity_alt;
	lua_rawgeti(L, -1, 1);
	velocity_alt = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			API::joint->velocity_alt = velocity_alt;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_axis(lua_State* L)
{
	Vector3 axis;
	lua_rawgeti(L, -1, 1);
	axis.x = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	axis.y = lua_tonumber(L, -1); 
	lua_rawgeti(L, -3, 3);
	axis.z = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			API::joint->axis = axis;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_axis_alt(lua_State* L)
{
	Vector3 axis_alt;
	lua_rawgeti(L, -1, 1);
	axis_alt.x = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	axis_alt.y = lua_tonumber(L, -1); 
	lua_rawgeti(L, -3, 3);
	axis_alt.z = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			API::joint->axis_alt = axis_alt;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_range(lua_State* L)
{
	lua_Number range[2];
		
	lua_rawgeti(L, -1, 1);
	range[0] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	range[1] = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			API::joint->range[0] = range[0];
			API::joint->range[1] = range[1];
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_range_alt(lua_State* L)
{
	lua_Number range_alt[2];
	lua_rawgeti(L, -1, 1);
	range_alt[0] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	range_alt[1] = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			API::joint->range_alt[0] = range_alt[0];
			API::joint->range_alt[1] = range_alt[1];
		} break;
	}
	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_connections(lua_State* L)
{
	std::string connections[2];
		
	lua_rawgeti(L, -1, 1);
	connections[0] = lua_tostring(L, -1); 
	lua_rawgeti(L, -2, 2);
	connections[1] = lua_tostring(L, -1); 

	switch(DataContext) {
		case JointContext: {
			API::joint->connections[0] = connections[0];
			API::joint->connections[1] = connections[1];
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_connection_type(lua_State* L)
{
	std::string connectionType = lua_tostring(L, -1); 	

	switch(DataContext) {
		case JointContext: {
			if ("hinge" == connectionType) {
				API::joint->connectionType = Hinge;
			} else if ("slider" == connectionType) {
				API::joint->connectionType = Slider;
			} else if ("universal" == connectionType) {
				API::joint->connectionType = Universal;
			} else if ("hinge2" == connectionType) {
				API::joint->connectionType = Hinge2;
			} else {
			// Error Handling
			}
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

static int API_GetWindowSize(lua_State* L)
{
	lua_newtable(L);
	lua_pushnumber(L, window.width);
	lua_setfield(L, -2, "width");
	lua_pushnumber(L, window.height);
	lua_setfield(L, -2, "height");
	return 1;
}

static int API_dofile(lua_State* L)
{
	const char* filepath = lua_tostring(L, -1);
	lua_Number result = luau_dofile(L, filepath);
	lua_gettop(L);
	return result;
}

static int API_require(lua_State* L)
{
	const char* filename = lua_tostring(L, -1);
	lua_Number result = luau_require(L, filename);
	lua_gettop(L);
	return 1;
}
static int API_loadmod(lua_State* L)
{
	const char* modpath = lua_tostring(L, -1);
	lua_Number result = API::loadmod(modpath);
	lua_pushnumber(L, result);
	return 1;
}

static int API_loadscript(lua_State* L)
{
	const char* scriptpath = lua_tostring(L, -1);
	lua_Number result = API::loadscript(scriptpath);
	lua_pushnumber(L, result);
	return 1;
}

static const luaL_Reg api_main[] {
	{"SetCallback", API_SetCallback},
	{"GetCallback", API_GetCallback},

	{"GetWindowSize", API_GetWindowSize},

	{"dofile", API_dofile},
	{"require", API_require},

	{"loadscript", API_loadscript},
	{"loadmod", API_loadmod},

	{"turnframes", API_turnframes},
	{"numplayers", API_numplayers},
	{"friction", API_friction},
	{"engagedistance", API_engagedistance},
	{"engageheight", API_engageheight},
	{"engagepos", API_engagepos},
	{"engagerot", API_engagerot},
	{"gravity", API_gravity},

	{"mod", API_mod},
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

static const char* events[] = {
	"NewGame",
	"Freeze",
	"Step",
	"Update",
	"Draw",
	"Draw2D",
	"Draw3D",
	"MouseButtonPressed",
	"MouseButtonDown",
	"MouseButtonUp",

	"NearCallback",
	"FileDropped",
	"Console",
};

int luaopen_api_main(lua_State* L) {
	luaL_register(L, "API", api_main);

	lua_getglobal(L, "API");
	for (auto event : events) {
		lua_newtable(L);
		lua_setfield(L, -2, event);
	}
	lua_pop(L, 1);
	return 1;
}
