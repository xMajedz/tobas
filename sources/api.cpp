#include <iostream>
#include <api.h>

#define PRINT(X) std::cout << "-- TEST " << X << " --" << std::endl;
#define PRINT_A PRINT("A")
#define PRINT_B PRINT("B")
#define PRINT_C PRINT("C")

extern API api;
extern Window window;
extern Console console;

API::API(Game* game_ptr)
{
	game = game_ptr;
}

int GAME_get_game_frame(lua_State* L)
{
	lua_pushnumber(L, api.game->state.game_frame);
	return 1;
}

int GAME_get_reaction_time(lua_State* L)
{
	lua_pushnumber(L, api.game->rules.reaction_time);
	return 1;
}

int GAME_get_reaction_count(lua_State* L)
{
	lua_pushnumber(L, api.game->state.reaction_count);
	return 1;
}

int luaopen_api_game(lua_State* L)
{
	luaL_register(L, "GAME", api_game);
	return 1;
}

enum GameContext {
	NoContext,
	ObjectContext,
	PlayerContext,
	BodyContext,
	JointContext,
} DataContext = NoContext;


int API_turnframes(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number turnframes = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			api.game->rules.turnframes = turnframes;
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

int API_numplayers(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number numplayers = lua_tonumber(L, -1); 

	switch(DataContext) {
		case NoContext: {
			api.game->rules.numplayers = numplayers;
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

int API_friction(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number friction = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			api.game->rules.friction = friction;
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

int API_engagedistance(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number distance = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			api.game->rules.engagedistance = distance;
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

int API_engageheight(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number height = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			api.game->rules.engageheight = height;
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

int API_engagepos(lua_State* L)
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
			api.player->use_engagepos = true;
			api.player->engagepos.x = pos.x;
			api.player->engagepos.y = pos.y;
			api.player->engagepos.z = pos.z;
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

int API_engagerot(lua_State* L)
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
			api.player->engagerot.x = rot.x;
			api.player->engagerot.y = rot.y;
			api.player->engagerot.z = rot.z;
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

int API_gravity(lua_State* L)
{
	lua_Number gravity[3];
	lua_rawgeti(L, -1, 1);
	gravity[0] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	gravity[1] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -3, 3);
	gravity[2] = lua_tonumber(L, -1); 

	switch(DataContext) {
		case NoContext: {
			api.game->rules.gravity[0] = gravity[0];
			api.game->rules.gravity[1] = gravity[1];
			api.game->rules.gravity[2] = gravity[2];
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

int API_object(lua_State* L)
{
	DataContext = ObjectContext;
	std::string name = lua_tostring(L, -1);
	api.game->objects[name].name = name;
	api.object = &(api.game->objects[name]);

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_player(lua_State* L)
{
	DataContext = PlayerContext;
	std::string name = lua_tostring(L, -1);

	if (api.game->players.size() > api.game->rules.numplayers + 1) {
		lua_Number result = 1;
		lua_pushnumber(L, result);
		return 1;
	}

	api.game->players[name].name = name;
	api.player = &(api.game->players[name]);

	switch (api.game->players.size()) {
		case 1: {
			api.game->state.selected_player = name;
			api.player->joint_color = MAROON;
			api.player->body_category_bits = 1<<2;
			api.player->joint_category_bits = 1<<3;
		} break;
		case 2: {
			api.player->joint_color = DARKBLUE;
			api.player->body_category_bits = 1<<4;
			api.player->joint_category_bits = 1<<5;
		} break;
		case 3: {
			api.player->joint_color = DARKGREEN;
			api.player->body_category_bits = 1<<6;
			api.player->joint_category_bits = 1<<7;
		} break;
		case 4: {
			api.player->joint_color = DARKPURPLE;
			api.player->body_category_bits = 1<<8;
			api.player->joint_category_bits = 1<<9;
		} break;
	}

	api.player->body_collide_bits = 0b0001; 
	api.player->joint_collide_bits = 0b0001;

	api.player->ghost = true;
	api.player->ghost_color = api.player->joint_color;
	api.player->ghost_color.a = 55;

	api.player->joint_select_color = WHITE;

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_body(lua_State* L)
{
	DataContext = BodyContext;
	std::string name = lua_tostring(L, -1);
	api.player->body[name].name = name;
	api.body = &(api.player->body[name]);

	//api.body->world = api.game->world;
	//api.body->space = api.game->space;

	api.body->color = api.player->body_color;
	api.body->ghost_color = api.player->ghost_color;

	api.body->orientation.w = 1.00;
	api.body->orientation.x = 0.00;
	api.body->orientation.y = 0.00;
	api.body->orientation.z = 0.00;

	api.body->freeze.orientation.w = 1.00;
	api.body->freeze.orientation.x = 0.00;
	api.body->freeze.orientation.y = 0.00;
	api.body->freeze.orientation.z = 0.00;

	api.body->freeze.linear_vel.x = 0.00;
	api.body->freeze.linear_vel.y = 0.00;
	api.body->freeze.linear_vel.z = 0.00;

	api.body->freeze.angular_vel.x = 0.00;
	api.body->freeze.angular_vel.y = 0.00;
	api.body->freeze.angular_vel.z = 0.00;

	api.body->category_bits = api.player->body_category_bits;
	api.body->collide_bits = api.player->body_collide_bits;

	api.body->ghost = true;
	api.body->static_state = false;

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_joint(lua_State* L)
{
	DataContext = JointContext;
	std::string name = lua_tostring(L, -1);
	api.player->joint[name].name = name;
	api.joint = &(api.player->joint[name]);

	//api.joint->world = api.game->world;
	//api.joint->space = api.game->space;

	api.joint->color = api.player->joint_color;
	api.joint->ghost_color = api.player->ghost_color;
	api.joint->select_color = api.player->joint_select_color;

	api.joint->orientation.w = 1.00;
	api.joint->orientation.x = 0.00;
	api.joint->orientation.y = 0.00;
	api.joint->orientation.z = 0.00;

	api.joint->freeze.orientation.w = 1.00;
	api.joint->freeze.orientation.x = 0.00;
	api.joint->freeze.orientation.y = 0.00;
	api.joint->freeze.orientation.z = 0.00;

	api.joint->freeze.linear_vel.x = 0.00;
	api.joint->freeze.linear_vel.y = 0.00;
	api.joint->freeze.linear_vel.z = 0.00;

	api.joint->freeze.angular_vel.x = 0.00;
	api.joint->freeze.angular_vel.y = 0.00;
	api.joint->freeze.angular_vel.z = 0.00;

	api.joint->category_bits = api.player->joint_category_bits;
	api.joint->collide_bits = api.player->joint_collide_bits;

	api.joint->state = RELAX;
	api.joint->state_alt = RELAX;

	api.joint->ghost = true;
	api.joint->static_state= false;

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_shape(lua_State* L)
{
	std::string shape = lua_tostring(L, -1);

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			if ("box" == shape) {
				api.object->shape = Box;
			} else if ("sphere" == shape) {
					api.object->shape = Sphere;
			} else if ("capsule" == shape) {
					api.object->shape = Capsule;
			} else if ("cylinder" == shape) {
					api.object->shape = Cylinder;
			} else  {
				//Error Handling
			}
		} break;
		case BodyContext: {
			if ("box" == shape) {
				api.body->shape = Box;
			} else if ("sphere" == shape) {
				api.body->shape = Sphere;
			} else if ("capsule" == shape) {
				api.body->shape = Capsule;
			} else if ("cylinder" == shape) {
				api.body->shape = Cylinder;
			} else  {
				//Error Handling
			}
		} break;
		case JointContext: {
			if ("box"  == shape) {
				api.joint->shape = Box;
			} else if ("sphere" == shape) {
				api.joint->shape = Sphere;
			} else if ("capsule" == shape) {
				api.joint->shape = Capsule;
			} else if ("cylinder" == shape) {
				api.joint->shape = Cylinder;
			} else  {
				//Error Handling
			}
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_position(lua_State* L)
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
			api.object->position.x = position.x;
			api.object->position.y = position.y;
			api.object->position.z = position.z;
			api.object->freeze.position.x = position.x;
			api.object->freeze.position.y = position.y;
			api.object->freeze.position.z = position.z;
		} break;
		case BodyContext: {
			if (api.player->use_engagepos) {
				position.x = position.x + api.player->engagepos.x;
				position.y = position.y + api.player->engagepos.y;
				position.z = position.z + api.player->engagepos.z;
			}
			api.body->position.x = position.x;
			api.body->position.y = position.y;
			api.body->position.z = position.z;
			api.body->freeze.position.x = position.x;
			api.body->freeze.position.y = position.y;
			api.body->freeze.position.z = position.z;
		} break;
		case JointContext: {
			if (api.player->use_engagepos) {
				position.x = position.x + api.player->engagepos.x;
				position.y = position.y + api.player->engagepos.y;
				position.z = position.z + api.player->engagepos.z;
			}
			api.joint->position.x = position.x;
			api.joint->position.y = position.y;
			api.joint->position.z = position.z;
			api.joint->freeze.position.x = position.x;
			api.joint->freeze.position.y = position.y;
			api.joint->freeze.position.z = position.z;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_orientation(lua_State* L)
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
			api.object->orientation = (Vector4){
				orientation.x,
				orientation.y,
				orientation.z,
				orientation.w,
			};

			api.object->freeze.orientation = (Vector4){
				orientation.x,
				orientation.y,
				orientation.z,
				orientation.w,
			};

		} break;
		case BodyContext: {
			api.body->orientation = (Vector4){
				orientation.x,
				orientation.y,
				orientation.z,
				orientation.w,
			};

			api.body->freeze.orientation = (Vector4){
				orientation.x,
				orientation.y,
				orientation.z,
				orientation.w,
			};
		} break;
		case JointContext: {

			api.joint->orientation = (Vector4){
				orientation.x,
				orientation.y,
				orientation.z,
				orientation.w,
			};

			api.joint->freeze.orientation = (Vector4){
				orientation.x,
				orientation.y,
				orientation.z,
				orientation.w,
			};
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_sides(lua_State* L)
{
	lua_Number sides[3];
	lua_rawgeti(L, -1, 1);
	sides[0] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	sides[1] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -3, 3);
	sides[2] = lua_tonumber(L, -1); 

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			api.object->sides.x = sides[0];
			api.object->sides.y = sides[1];
			api.object->sides.z = sides[2];
		} break;
		case BodyContext: {
			api.body->sides.x = sides[0];
			api.body->sides.y = sides[1];
			api.body->sides.z = sides[2];
		} break;
		case JointContext: {
			api.joint->sides.x = sides[0];
			api.joint->sides.y = sides[1];
			api.joint->sides.z = sides[2];
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_density(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number density = lua_tonumber(L, -1); 

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			api.object->density = density;
		} break;
		case BodyContext: {
			api.body->density = density;
		} break;
		case JointContext: {
			api.joint->density = density;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_static(lua_State* L)
{
	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			api.object->static_state = true;
			api.object->color = BLACK;
			api.object->category_bits = 0b0001;
			api.object->collide_bits = 0b0000;
		} break;
		case BodyContext: {
			api.body->static_state = true;
		} break;
		case JointContext: {
			api.joint->static_state = true;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_radius(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number radius = lua_tonumber(L, -1); 

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			api.object->radius = radius;
		} break;
		case BodyContext: {
			api.body->radius = radius;
		} break;
		case JointContext: {
			api.joint->radius = radius;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_length(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number length = lua_tonumber(L, -1); 

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			api.object->length = length;
		} break;
		case BodyContext: {
			api.body->length = length;
		} break;
		case JointContext: {
			api.joint->length = length;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_strength(lua_State* L)
{
	lua_Number strength;
	lua_rawgeti(L, -1, 1);
	strength = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			api.joint->strength = strength;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_strength_alt(lua_State* L)
{
	lua_Number strength_alt;
	lua_rawgeti(L, -1, 1);
	strength_alt = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			api.joint->strength_alt = strength_alt;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}


int API_velocity(lua_State* L)
{
	lua_Number velocity;
	lua_rawgeti(L, -1, 1);
	velocity = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			api.joint->velocity = velocity;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_velocity_alt(lua_State* L)
{
	lua_Number velocity_alt;
	lua_rawgeti(L, -1, 1);
	velocity_alt = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			api.joint->velocity_alt = velocity_alt;
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_axis(lua_State* L)
{
	lua_Number axis[3];
	lua_rawgeti(L, -1, 1);
	axis[0] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	axis[1] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -3, 3);
	axis[2] = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			api.joint->axis[0] = axis[0];
			api.joint->axis[1] = axis[1];
			api.joint->axis[2] = axis[2];
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_axis_alt(lua_State* L)
{
	lua_Number axis_alt[3];
	lua_rawgeti(L, -1, 1);
	axis_alt[0] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	axis_alt[1] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -3, 3);
	axis_alt[2] = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			api.joint->axis_alt[0] = axis_alt[0];
			api.joint->axis_alt[1] = axis_alt[1];
			api.joint->axis_alt[2] = axis_alt[2];
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_range(lua_State* L)
{
	lua_Number range[2];
		
	lua_rawgeti(L, -1, 1);
	range[0] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	range[1] = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			api.joint->range[0] = range[0];
			api.joint->range[1] = range[1];
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_range_alt(lua_State* L)
{
	lua_Number range_alt[2];
	lua_rawgeti(L, -1, 1);
	range_alt[0] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	range_alt[1] = lua_tonumber(L, -1); 

	switch(DataContext) {
		case JointContext: {
			api.joint->range_alt[0] = range_alt[0];
			api.joint->range_alt[1] = range_alt[1];
		} break;
	}
	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_connections(lua_State* L)
{
	std::string connections[2];
		
	lua_rawgeti(L, -1, 1);
	connections[0] = lua_tostring(L, -1); 
	lua_rawgeti(L, -2, 2);
	connections[1] = lua_tostring(L, -1); 

	switch(DataContext) {
		case JointContext: {
			api.joint->connections[0] = connections[0];
			api.joint->connections[1] = connections[1];
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_connection_type(lua_State* L)
{
	std::string connectionType = lua_tostring(L, -1); 	

	switch(DataContext) {
		case JointContext: {
			if ("hinge" == connectionType) {
				api.joint->connectionType = Hinge;
			} else if ("slider" == connectionType) {
				api.joint->connectionType = Slider;
			} else if ("universal" == connectionType) {
				api.joint->connectionType = Universal;
			} else if ("hinge2" == connectionType) {
				api.joint->connectionType = Hinge2;
			} else {
			// Error Handling
			}
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_GetWindowSize(lua_State* L)
{
	lua_newtable(L);
	lua_pushnumber(L, window.width);
	lua_setfield(L, -2, "width");
	lua_pushnumber(L, window.height);
	lua_setfield(L, -2, "height");
	return 1;
}

int API_dofile(lua_State* L)
{
	const char* filepath = lua_tostring(L, -1);
	lua_Number result = luau_dofile(L, filepath);
	lua_gettop(L);
	return result;
}

int API_require(lua_State* L)
{
	const char* filename = lua_tostring(L, -1);
	lua_Number result = luau_require(L, filename);
	lua_gettop(L);
	return 1;
}

int loadmod (lua_State* L, const char* modpath)
{
	return luau_dofile(
		L,
		TextFormat("mods/%s", modpath),
		TextFormat("%s:%s", "loadmod", modpath)
	);
}

int API_loadmod(lua_State* L)
{
	const char* modpath = lua_tostring(L, -1);
	lua_Number result = loadmod(L, modpath);
	lua_pushnumber(L, result);
	return 1;
}

int loadscript (lua_State* L, const char* scriptpath)
{
	const char* path = TextFormat("scripts/%s", scriptpath);
	return luau_dofile(
		L,
		path,
		TextFormat("%s:%s", "loadscript", scriptpath)
	);
}

int API_loadscript(lua_State* L)
{
	const char* scriptpath = lua_tostring(L, -1);
	lua_Number result = loadscript(L, scriptpath);
	lua_pushnumber(L, result);
	return 1;
}

static const luaL_Reg api_main[] {
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

int luaopen_api_main(lua_State* L) {
	luaL_register(L, "API", api_main);
	return 1;
}
