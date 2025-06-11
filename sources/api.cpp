#include <api.hpp>

int API_turnframes(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number turnframes = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			game.rules.turnframes = turnframes;
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
			game.rules.numplayers = numplayers;
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
			game.rules.friction = friction;
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
			game.rules.engagedistance = distance;
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
			game.rules.engageheight = height;
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
	lua_Number pos[3];
	lua_rawgeti(L, -1, 1);
	pos[0] = lua_tonumber(L, -1);
	lua_rawgeti(L, -2, 2);
	pos[1] = lua_tonumber(L, -1);
	lua_rawgeti(L, -3, 3);
	pos[2] = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case PlayerContext: {
			game.players[player_key].engagepos[0] = pos[0];
			game.players[player_key].engagepos[1] = pos[1];
			game.players[player_key].engagepos[2] = pos[2];
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
	lua_Number rot[3];

	lua_rawgeti(L, -1, 1);
	rot[0] = lua_tonumber(L, -1);
	lua_rawgeti(L, -2, 2);
	rot[1] = lua_tonumber(L, -1);
	lua_rawgeti(L, -3, 3);
	rot[2] = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case PlayerContext: {
			game.players[player_key].engagerot[0] = rot[0];
			game.players[player_key].engagerot[1] = rot[1];
			game.players[player_key].engagerot[2] = rot[2];
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
			game.rules.gravity[0] = gravity[0];
			game.rules.gravity[1] = gravity[1];
			game.rules.gravity[2] = gravity[2];
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
	object_key = name;
	game.objects[object_key].name = name;
	game.objects[object_key].color = DynamicObjectColor;

	game.objects[object_key].orientation[0] = 1.00;
	game.objects[object_key].orientation[1] = 0.00;
	game.objects[object_key].orientation[2] = 0.00;
	game.objects[object_key].orientation[3] = 0.00;

	game.objects[object_key].freeze.orientation[0] = 1.00;
	game.objects[object_key].freeze.orientation[1] = 0.00;
	game.objects[object_key].freeze.orientation[2] = 0.00;
	game.objects[object_key].freeze.orientation[3] = 0.00;

	game.objects[object_key].freeze.linear_vel[0] = 0.00;
	game.objects[object_key].freeze.linear_vel[1] = 0.00;
	game.objects[object_key].freeze.linear_vel[2] = 0.00;

	game.objects[object_key].freeze.angular_vel[0] = 0.00;
	game.objects[object_key].freeze.angular_vel[1] = 0.00;
	game.objects[object_key].freeze.angular_vel[2] = 0.00;

	game.objects[object_key].category_bits = 0b0001;
	game.objects[object_key].collide_bits = 0b0001;

	game.objects[object_key].select = false;
	game.objects[object_key].static_state = false;

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_player(lua_State* L)
{
	DataContext = PlayerContext;
	std::string name = lua_tostring(L, -1);
	if (game.players.size() > game.rules.numplayers + 1) {
		lua_Number result = 1;
		lua_pushnumber(L, result);
		return 1;
	}
	player_key = name;
	game.players[player_key].name = name;

	switch (game.players.size()) {
		case 1: {
			game.state.selected_player = player_key;
			game.players[player_key].joint_color = MAROON;
			game.players[player_key].body_category_bits = 1<<2;
			game.players[player_key].joint_category_bits = 1<<3;
		} break;
		case 2: {
			game.players[player_key].joint_color = DARKBLUE;
			game.players[player_key].body_category_bits = 1<<4;
			game.players[player_key].joint_category_bits = 1<<5;
		} break;
		case 3: {
			game.players[player_key].joint_color = DARKGREEN;
			game.players[player_key].body_category_bits = 1<<6;
			game.players[player_key].joint_category_bits = 1<<7;
		} break;
		case 4: {
			game.players[player_key].joint_color = DARKPURPLE;
			game.players[player_key].body_category_bits = 1<<8;
			game.players[player_key].joint_category_bits = 1<<9;
		} break;
	}

	game.players[player_key].body_collide_bits = 0b0001; 
	game.players[player_key].joint_collide_bits = 0b0001;

	game.players[player_key].ghost = true;
	game.players[player_key].ghost_color = game.players[player_key].joint_color;
	game.players[player_key].ghost_color.a = 55;

	game.players[player_key].joint_select_color = WHITE;

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_body(lua_State* L)
{
	DataContext = BodyContext;
	std::string name = lua_tostring(L, -1);
	body_key = name;
	game.players[player_key].body[body_key].name = name;

	game.players[player_key].body[body_key].color = game.players[player_key].body_color;
	game.players[player_key].body[body_key].ghost_color = game.players[player_key].ghost_color;

	game.players[player_key].body[body_key].orientation[0] = 1.00;
	game.players[player_key].body[body_key].orientation[1] = 0.00;
	game.players[player_key].body[body_key].orientation[2] = 0.00;
	game.players[player_key].body[body_key].orientation[3] = 0.00;

	game.players[player_key].body[body_key].freeze.orientation[0] = 1.00;
	game.players[player_key].body[body_key].freeze.orientation[1] = 0.00;
	game.players[player_key].body[body_key].freeze.orientation[2] = 0.00;
	game.players[player_key].body[body_key].freeze.orientation[3] = 0.00;

	game.players[player_key].body[body_key].freeze.linear_vel[0] = 0.00;
	game.players[player_key].body[body_key].freeze.linear_vel[1] = 0.00;
	game.players[player_key].body[body_key].freeze.linear_vel[2] = 0.00;

	game.players[player_key].body[body_key].freeze.angular_vel[0] = 0.00;
	game.players[player_key].body[body_key].freeze.angular_vel[1] = 0.00;
	game.players[player_key].body[body_key].freeze.angular_vel[2] = 0.00;

	game.players[player_key].body[body_key].category_bits = game.players[player_key].body_category_bits;
	game.players[player_key].body[body_key].collide_bits = game.players[player_key].body_collide_bits;

	game.players[player_key].body[body_key].ghost = true;
	game.players[player_key].body[body_key].static_state = false;

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_joint(lua_State* L)
{
	DataContext = JointContext;
	std::string name = lua_tostring(L, -1);
	joint_key = name;
	game.players[player_key].joint[joint_key].name = name;

	game.players[player_key].joint[joint_key].color = game.players[player_key].joint_color;
	game.players[player_key].joint[joint_key].ghost_color = game.players[player_key].ghost_color;
	game.players[player_key].joint[joint_key].select_color = game.players[player_key].joint_select_color;

	game.players[player_key].joint[joint_key].orientation[0] = 1.00;
	game.players[player_key].joint[joint_key].orientation[1] = 0.00;
	game.players[player_key].joint[joint_key].orientation[2] = 0.00;
	game.players[player_key].joint[joint_key].orientation[3] = 0.00;

	game.players[player_key].joint[joint_key].freeze.orientation[0] = 1.00;
	game.players[player_key].joint[joint_key].freeze.orientation[1] = 0.00;
	game.players[player_key].joint[joint_key].freeze.orientation[2] = 0.00;
	game.players[player_key].joint[joint_key].freeze.orientation[3] = 0.00;

	game.players[player_key].joint[joint_key].freeze.linear_vel[0] = 0.00;
	game.players[player_key].joint[joint_key].freeze.linear_vel[1] = 0.00;
	game.players[player_key].joint[joint_key].freeze.linear_vel[2] = 0.00;

	game.players[player_key].joint[joint_key].freeze.angular_vel[0] = 0.00;
	game.players[player_key].joint[joint_key].freeze.angular_vel[1] = 0.00;
	game.players[player_key].joint[joint_key].freeze.angular_vel[2] = 0.00;

	game.players[player_key].joint[joint_key].category_bits = game.players[player_key].joint_category_bits;
	game.players[player_key].joint[joint_key].collide_bits = game.players[player_key].joint_collide_bits;

	game.players[player_key].joint[joint_key].state = RELAX;
	game.players[player_key].joint[joint_key].state_alt = RELAX;

	game.players[player_key].joint[joint_key].ghost = true;
	game.players[player_key].joint[joint_key].static_state= false;

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
				game.objects[object_key].shape = Box;
			} else if ("sphere" == shape) {
					game.objects[object_key].shape = Sphere;
			} else if ("capsule" == shape) {
					game.objects[object_key].shape = Capsule;
			} else if ("cylinder" == shape) {
					game.objects[object_key].shape = Cylinder;
			} else  {
				//Error Handling
			}
		} break;
		case BodyContext: {
			if ("box" == shape) {
				game.players[player_key].body[body_key].shape = Box;
			} else if ("sphere" == shape) {
				game.players[player_key].body[body_key].shape = Sphere;
			} else if ("capsule" == shape) {
				game.players[player_key].body[body_key].shape = Capsule;
			} else if ("cylinder" == shape) {
				game.players[player_key].body[body_key].shape = Cylinder;
			} else  {
				//Error Handling
			}
		} break;
		case JointContext: {
			if ("box"  == shape) {
				game.players[player_key].joint[joint_key].shape = Box;
			} else if ("sphere" == shape) {
				game.players[player_key].joint[joint_key].shape = Sphere;
			} else if ("capsule" == shape) {
				game.players[player_key].joint[joint_key].shape = Capsule;
			} else if ("cylinder" == shape) {
				game.players[player_key].joint[joint_key].shape = Cylinder;
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
	lua_Number position[3];
	lua_rawgeti(L, -1, 1);
	position[0] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	position[1] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -3, 3);
	position[2] = lua_tonumber(L, -1); 
	
	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			game.objects[object_key].position[0] = position[0];
			game.objects[object_key].position[1] = position[1];
			game.objects[object_key].position[2] = position[2];
			game.objects[object_key].freeze.position[0] = position[0];
			game.objects[object_key].freeze.position[1] = position[1];
			game.objects[object_key].freeze.position[2] = position[2];
		} break;
		case BodyContext: {
			if (game.players[player_key].engagepos) {
				position[0] = position[0] + game.players[player_key].engagepos[0];
				position[1] = position[1] + game.players[player_key].engagepos[1];
				position[2] = position[2] + game.players[player_key].engagepos[2];
			}
			game.players[player_key].body[body_key].position[0] = position[0];
			game.players[player_key].body[body_key].position[1] = position[1];
			game.players[player_key].body[body_key].position[2] = position[2];
			game.players[player_key].body[body_key].freeze.position[0] = position[0];
			game.players[player_key].body[body_key].freeze.position[1] = position[1];
			game.players[player_key].body[body_key].freeze.position[2] = position[2];
		} break;
		case JointContext: {
			if (game.players[player_key].engagepos) {
				position[0] = position[0] + game.players[player_key].engagepos[0];
				position[1] = position[1] + game.players[player_key].engagepos[1];
				position[2] = position[2] + game.players[player_key].engagepos[2];
			}
			game.players[player_key].joint[joint_key].position[0] = position[0];
			game.players[player_key].joint[joint_key].position[1] = position[1];
			game.players[player_key].joint[joint_key].position[2] = position[2];
			game.players[player_key].joint[joint_key].freeze.position[0] = position[0];
			game.players[player_key].joint[joint_key].freeze.position[1] = position[1];
			game.players[player_key].joint[joint_key].freeze.position[2] = position[2];
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int API_orientation(lua_State* L)
{
	lua_Number orientation[4];
	lua_rawgeti(L, -1, 1);
	orientation[0] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	orientation[1] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -3, 3);
	orientation[2] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -4, 4);
	orientation[3] = lua_tonumber(L, -1);

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			game.objects[object_key].orientation[0] = orientation[0];
			game.objects[object_key].orientation[1] = orientation[1];
			game.objects[object_key].orientation[2] = orientation[2];
			game.objects[object_key].orientation[3] = orientation[3];

			game.objects[object_key].freeze.orientation[0] = orientation[0];
			game.objects[object_key].freeze.orientation[1] = orientation[1];
			game.objects[object_key].freeze.orientation[2] = orientation[2];
			game.objects[object_key].freeze.orientation[3] = orientation[3];
		} break;
		case BodyContext: {
			game.players[player_key].body[body_key].orientation[0] = orientation[0];
			game.players[player_key].body[body_key].orientation[1] = orientation[1];
			game.players[player_key].body[body_key].orientation[2] = orientation[2];
			game.players[player_key].body[body_key].orientation[3] = orientation[3];

			game.players[player_key].body[body_key].freeze.orientation[0] = orientation[0];
			game.players[player_key].body[body_key].freeze.orientation[1] = orientation[1];
			game.players[player_key].body[body_key].freeze.orientation[2] = orientation[2];
			game.players[player_key].body[body_key].freeze.orientation[3] = orientation[3];
		} break;
		case JointContext: {
			game.players[player_key].joint[joint_key].orientation[0] = orientation[0];
			game.players[player_key].joint[joint_key].orientation[1] = orientation[1];
			game.players[player_key].joint[joint_key].orientation[2] = orientation[2];
			game.players[player_key].joint[joint_key].orientation[3] = orientation[3];
	
			game.players[player_key].joint[joint_key].freeze.orientation[0] = orientation[0];
			game.players[player_key].joint[joint_key].freeze.orientation[1] = orientation[1];
			game.players[player_key].joint[joint_key].freeze.orientation[2] = orientation[2];
			game.players[player_key].joint[joint_key].freeze.orientation[3] = orientation[3];
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
			game.objects[object_key].sides[0] = sides[0];
			game.objects[object_key].sides[1] = sides[1];
			game.objects[object_key].sides[2] = sides[2];
		} break;
		case BodyContext: {
			game.players[player_key].body[body_key].sides[0] = sides[0];
			game.players[player_key].body[body_key].sides[1] = sides[1];
			game.players[player_key].body[body_key].sides[2] = sides[2];
		} break;
		case JointContext: {
			game.players[player_key].joint[joint_key].sides[0] = sides[0];
			game.players[player_key].joint[joint_key].sides[1] = sides[1];
			game.players[player_key].joint[joint_key].sides[2] = sides[2];
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int density(lua_State* L)
{
	lua_rawgeti(L, -1, 1);
	lua_Number density = lua_tonumber(L, -1); 

	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			game.objects[object_key].density = density;
		} break;
		case BodyContext: {
			game.players[player_key].body[body_key].density = density;
		} break;
		case JointContext: {
			game.players[player_key].joint[joint_key].density = density;
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
			game.objects[object_key].static_state = true;
			game.objects[object_key].color = StaticObjectColor;
			game.objects[object_key].category_bits = 0b0001;
			game.objects[object_key].collide_bits = 0b0000;
		} break;
		case BodyContext: {
			game.players[player_key].body[body_key].static_state = true;
		} break;
		case JointContext: {
			game.players[player_key].joint[joint_key].static_state = true;
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
			game.objects[object_key].radius = radius;
		} break;
		case BodyContext: {
			game.players[player_key].body[body_key].radius = radius;
		} break;
		case JointContext: {
			game.players[player_key].joint[joint_key].radius = radius;
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
			game.objects[object_key].length = length;
		} break;
		case BodyContext: {
			game.players[player_key].body[body_key].length = length;
		} break;
		case JointContext: {
			game.players[player_key].joint[joint_key].length = length;
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
			game.players[player_key].joint[joint_key].strength = strength;
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
			game.players[player_key].joint[joint_key].strength_alt = strength_alt;
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
			game.players[player_key].joint[joint_key].velocity = velocity;
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
			game.players[player_key].joint[joint_key].velocity_alt = velocity_alt;
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
			game.players[player_key].joint[joint_key].axis[0] = axis[0];
			game.players[player_key].joint[joint_key].axis[1] = axis[1];
			game.players[player_key].joint[joint_key].axis[2] = axis[2];
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
			game.players[player_key].joint[joint_key].axis_alt[0] = axis_alt[0];
			game.players[player_key].joint[joint_key].axis_alt[1] = axis_alt[1];
			game.players[player_key].joint[joint_key].axis_alt[2] = axis_alt[2];
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
			game.players[player_key].joint[joint_key].range[0] = range[0];
			game.players[player_key].joint[joint_key].range[1] = range[1];
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
			game.players[player_key].joint[joint_key].range_alt[0] = range_alt[0];
			game.players[player_key].joint[joint_key].range_alt[1] = range_alt[1];
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
			game.players[player_key].joint[joint_key].connections[0] = connections[0];
			game.players[player_key].joint[joint_key].connections[1] = connections[1];
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
				game.players[player_key].joint[joint_key].connectionType = Hinge;
			} else if ("slider" == connectionType) {
				game.players[player_key].joint[joint_key].connectionType = Slider;
			} else if ("universal" == connectionType) {
				game.players[player_key].joint[joint_key].connectionType = Universal;
			} else if ("hinge2" == connectionType) {
				game.players[player_key].joint[joint_key].connectionType = Hinge2;
			} else {
			// Error Handling
			}
		} break;
	}

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int api_dofile(lua_State* L)
{
	const char* filename = lua_tostring(L, -1);
	lua_Number result = luau_dofile(L, filename, filename);
	lua_pushnumber(L, result);
	return 1;
}

void api_set(lua_State* L, lua_CFunction fn ,const char* fn_name)
{
	lua_pushcfunction(L, fn, fn_name);
	lua_setglobal(L, fn_name);
}

int luaopen_api_main(lua_State* L) {
	luaL_register(L, "API", api_main);
	return 1;
}
