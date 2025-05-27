#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

#include <ode/ode.h>
#include <lua.hpp>

enum GameContext {
	NoContext,
	objectContext,
	BodyContext,
	JointContext
} DataContext = NoContext;

enum Shape {
	Box,
	Sphere,
	Capsule,
	Cylinder,
	COMPOSITE,
};

enum JointType {
	Hinge,
	Slider,
	Universal,
	Hinge2,
};

enum JointState {
	RELAX,
	HOLD,
	FORWARD,
	BACKWARD,
};

enum Gamemode {
	FREEPLAY,
	REPLAY,
};

struct game {
	dWorldID world;
	dSpaceID space;
	dJointGroupID contactgroup;
	dGeomID floor;
	dReal engagedistance;
	dReal engageheight;
	dReal engageplayerpos[3];
	dReal engageplayerrot[3];
	dReal gravity[3];
	dReal friction = 1000.00;
	dReal freeze_time;
	dReal freeze_t;
	dReal unfreeze_time;
	dReal unfreeze_t;
	dReal step = 0.01;
	int gameframe;
	int turnframes = 10;
	int max_contacts = 8;
	Gamemode gamemode = FREEPLAY;
	std::string selected_joint = "NONE";
	bool freeze;
	bool pause = false;
} game;

class FreezeData {
	public:
	dReal position[3];
	dReal orientation[4];
	dReal angularVel[3];
	dReal linearVel[3];
};

struct Composite {
	std::string name;
	dBodyID dBody;
};

class Object {
	public:
	std::string name;
	Shape shape;
	dReal position[3];
	dReal orientation[4];
	dReal sides[3];
	dReal radius;
	dReal length;
	dReal density;
	dBodyID dBody;
	dGeomID dGeom;
	Composite composite;
	FreezeData freeze;
	bool static_state;
	bool isComposite;
};

class Body : public Object {

};

class Joint : public Object {
	public:
	std::string connections[2];
	JointType connectionType;
	JointState state;
	JointState state_alt;
	dJointID dJoint[2];
	dReal axis[3];
	dReal axis_alt[3];
	dReal range[2];
	dReal range_alt[2];
	dReal strength;
	dReal strength_alt;
	dReal velocity;
	dReal velocity_alt;
};

class FrameData : public FreezeData {
	public:
	std::map<std::string, Joint> joint;
};

enum PlayerPassiveStates {
	HOLD_ALL,
	RELAX_ALL,
	MIXED,
}; 

class Player {
	public:
	PlayerPassiveStates passive_states;
	PlayerPassiveStates passive_states_alt;
	Player() {
		passive_states = RELAX_ALL;
		passive_states_alt = RELAX_ALL;
	};
};

Player player;

std::string MSG;

Color DynamicobjectColor = (Color){ 0, 255, 0, 255 };
Color StaticobjectColor = (Color){ 51, 51, 51, 255 };
Color JointColor = (Color){ 155, 155, 155, 255 };
Color BodyColor = (Color){ 255, 255, 255, 255 };
Color GhostColor = (Color){ 51, 51, 51, 51 };

std::map<std::string, Object> object;
std::map<std::string, Body> body;
std::map<std::string, Joint> joint;

std::string object_key;
std::string body_key;
std::string joint_key;

unsigned long StaticobjectsCategoryBits = 0b0001;
unsigned long StaticobjectsCollideBits = 0b0000;

unsigned long DynamicobjectsCategoryBits = 0b0010;
unsigned long DynamicobjectsCollideBits = 0b0001;

unsigned long BodyCategoryBits = 0b0100;
unsigned long BodyCollideBits = 0b0001;

unsigned long JointCategoryBits = 0b1000;
unsigned long JointCollideBits = 0b0001;

static void nearCallback (void *, dGeomID o1, dGeomID o2)
{
	unsigned long cat1 = dGeomGetCategoryBits(o1);
	unsigned long col1 = dGeomGetCollideBits(o1);

	unsigned long cat2 = dGeomGetCategoryBits(o2);
	unsigned long col2 = dGeomGetCollideBits(o2);

 	if (!((cat1 & col2) || (cat2 & col1))) {
		return;
	}

	int i;

	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	dContact contact[game.max_contacts];

	for (i = 0; i < game.max_contacts; i++) {
		contact[i].surface.mode = dContactApprox1;
		contact[i].surface.mu = game.friction;
	}

	if (int numc = dCollide(o1, o2, game.max_contacts, &contact[0].geom, sizeof(dContact))) {
		for (i = 0; i < numc; i++) {
			dJointID c = dJointCreateContact(game.world, game.contactgroup, contact + i);
			dJointAttach(c, b1, b2);
		}
	}
}

int api_turnframes(lua_State* L)
{
	lua_Number turnframes;

	lua_rawgeti(L, -1, 1);
	turnframes = lua_tonumber(L, -1);

	switch(DataContext)
	{
		case NoContext: {
			game.turnframes = turnframes;
		} break;
		case objectContext: {
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

int api_friction(lua_State* L)
{
	lua_Number friction;

	lua_rawgeti(L, -1, 1);
	friction = lua_tonumber(L, -1);

	switch(DataContext)
	{
		case NoContext: {
			game.friction = friction;
		} break;
		case objectContext: {
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

int engagedistance(lua_State* L)
{
	lua_Number distance;

	lua_rawgeti(L, -1, 1);
	distance = lua_tonumber(L, -1);

	switch(DataContext)
	{
		case NoContext: {
			game.engagedistance = distance;
		} break;
		case objectContext: {
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

int engageheight(lua_State* L)
{
	lua_Number height;

	lua_rawgeti(L, -1, 1);
	height = lua_tonumber(L, -1);

	switch(DataContext)
	{
		case NoContext: {
			game.engageheight = height;
		} break;
		case objectContext: {
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

int engageplayerpos(lua_State* L)
{
	lua_Number pos[3];

	lua_rawgeti(L, -1, 1);
	pos[0] = lua_tonumber(L, -1);
	lua_rawgeti(L, -2, 2);
	pos[1] = lua_tonumber(L, -1);
	lua_rawgeti(L, -3, 3);
	pos[2] = lua_tonumber(L, -1);

	switch(DataContext)
	{
		case NoContext: {
			game.engageplayerpos[0] = pos[0];
			game.engageplayerpos[1] = pos[1];
			game.engageplayerpos[2] = pos[2];
		} break;
		case objectContext: {
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

int engageplayerrot(lua_State* L)
{
	lua_Number rot[3];

	lua_rawgeti(L, -1, 1);
	rot[0] = lua_tonumber(L, -1);
	lua_rawgeti(L, -2, 2);
	rot[1] = lua_tonumber(L, -1);
	lua_rawgeti(L, -3, 3);
	rot[2] = lua_tonumber(L, -1);

	switch(DataContext)
	{
		case NoContext: {
			game.engageplayerrot[0] = rot[0];
			game.engageplayerrot[1] = rot[1];
			game.engageplayerrot[2] = rot[2];
		} break;
		case objectContext: {
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

int gravity(lua_State* L)
{
	lua_Number gravity[3];
		
	lua_rawgeti(L, -1, 1);
	gravity[0] = lua_tonumber(L, -1); 

	lua_rawgeti(L, -2, 2);
	gravity[1] = lua_tonumber(L, -1); 

	lua_rawgeti(L, -3, 3);
	gravity[2] = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case NoContext: {
			game.gravity[0] = gravity[0];
			game.gravity[1] = gravity[1];
			game.gravity[2] = gravity[2];
		} break;
		case objectContext: {
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

int globalplane(lua_State* L)
{
	switch(DataContext)
	{
		case NoContext: {
			// Error Handling	
		} break;
		case objectContext: {
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

int api_object(lua_State* L)
{
	DataContext = objectContext;

	std::string name = lua_tostring(L, -1);
	
	object_key = name;

	object[object_key].name = name;

	object[object_key].orientation[0] = 1.00;
	object[object_key].orientation[1] = 0.00;
	object[object_key].orientation[2] = 0.00;
	object[object_key].orientation[3] = 0.00;

	object[object_key].freeze.orientation[0] = 1.00;
	object[object_key].freeze.orientation[1] = 0.00;
	object[object_key].freeze.orientation[2] = 0.00;
	object[object_key].freeze.orientation[3] = 0.00;

	object[object_key].freeze.linearVel[0] = 0.00;
	object[object_key].freeze.linearVel[1] = 0.00;
	object[object_key].freeze.linearVel[2] = 0.00;

	object[object_key].freeze.angularVel[0] = 0.00;
	object[object_key].freeze.angularVel[1] = 0.00;
	object[object_key].freeze.angularVel[2] = 0.00;

	object[object_key].static_state = false;
	object[object_key].isComposite = false;

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int api_body(lua_State* L)
{
	DataContext = BodyContext;

	std::string name = lua_tostring(L, -1);
	
	body_key = name;

	body[body_key].name = name;

	body[body_key].orientation[0] = 1.00;
	body[body_key].orientation[1] = 0.00;
	body[body_key].orientation[2] = 0.00;
	body[body_key].orientation[3] = 0.00;

	body[body_key].freeze.orientation[0] = 1.00;
	body[body_key].freeze.orientation[1] = 0.00;
	body[body_key].freeze.orientation[2] = 0.00;
	body[body_key].freeze.orientation[3] = 0.00;

	body[body_key].freeze.linearVel[0] = 0.00;
	body[body_key].freeze.linearVel[1] = 0.00;
	body[body_key].freeze.linearVel[2] = 0.00;

	body[body_key].freeze.angularVel[0] = 0.00;
	body[body_key].freeze.angularVel[1] = 0.00;
	body[body_key].freeze.angularVel[2] = 0.00;

	body[body_key].static_state = false;
	body[body_key].isComposite = false;

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int api_joint(lua_State* L)
{
	DataContext = JointContext;

	std::string name = lua_tostring(L, -1);

	joint_key = name;

	joint[joint_key].name = name;

	joint[joint_key].orientation[0] = 1.00;
	joint[joint_key].orientation[1] = 0.00;
	joint[joint_key].orientation[2] = 0.00;
	joint[joint_key].orientation[3] = 0.00;

	joint[joint_key].freeze.orientation[0] = 1.00;
	joint[joint_key].freeze.orientation[1] = 0.00;
	joint[joint_key].freeze.orientation[2] = 0.00;
	joint[joint_key].freeze.orientation[3] = 0.00;

	joint[joint_key].freeze.linearVel[0] = 0.00;
	joint[joint_key].freeze.linearVel[1] = 0.00;
	joint[joint_key].freeze.linearVel[2] = 0.00;

	joint[joint_key].freeze.angularVel[0] = 0.00;
	joint[joint_key].freeze.angularVel[1] = 0.00;
	joint[joint_key].freeze.angularVel[2] = 0.00;

	joint[joint_key].state = RELAX;
	joint[joint_key].state_alt = RELAX;

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int shape(lua_State* L)
{
	std::string shape = lua_tostring(L, -1);

	switch(DataContext)
	{
		case NoContext: {
			// Error Handling
		} break;
		case objectContext: {
			if ("box" == shape) {
				object[object_key].shape = Box;
			} else if ("sphere" == shape) {
					object[object_key].shape = Sphere;
			} else if ("capsule" == shape) {
					object[object_key].shape = Capsule;
			} else if ("cylinder" == shape) {
					object[object_key].shape = Cylinder;
			} else  {
				//Error Handling
			}
		} break;
		case BodyContext: {
			if ("box" == shape) {
				body[body_key].shape = Box;
			} else if ("sphere" == shape) {
				body[body_key].shape = Sphere;
			} else if ("capsule" == shape) {
				body[body_key].shape = Capsule;
			} else if ("cylinder" == shape) {
				body[body_key].shape = Cylinder;
			} else  {
				//Error Handling
			}
		} break;
		case JointContext: {
			if ("box"  == shape) {
				joint[joint_key].shape = Box;
			} else if ("sphere" == shape) {
				joint[joint_key].shape = Sphere;
			} else if ("capsule" == shape) {
				joint[joint_key].shape = Capsule;
			} else if ("cylinder" == shape) {
				joint[joint_key].shape = Cylinder;
			} else  {
				//Error Handling
			}
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int position(lua_State* L)
{
	lua_Number position[3];
		
	lua_rawgeti(L, -1, 1);
	position[0] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -2, 2);
	position[1] = lua_tonumber(L, -1); 
	lua_rawgeti(L, -3, 3);
	position[2] = lua_tonumber(L, -1); 
	
	switch(DataContext)
	{
		case NoContext: {
			// Error Handling
		} break;
		case objectContext: {
			object[object_key].position[0] = position[0];
			object[object_key].position[1] = position[1];
			object[object_key].position[2] = position[2];
			object[object_key].freeze.position[0] = position[0];
			object[object_key].freeze.position[1] = position[1];
			object[object_key].freeze.position[2] = position[2];
		} break;
		case BodyContext: {
			if (game.engageplayerpos) {
				body[body_key].position[0] = position[0] + game.engageplayerpos[0];
				body[body_key].position[1] = position[1] + game.engageplayerpos[1];
				body[body_key].position[2] = position[2] + game.engageplayerpos[2];
				body[body_key].freeze.position[0] = position[0] + game.engageplayerpos[0];
				body[body_key].freeze.position[1] = position[1] + game.engageplayerpos[1];
				body[body_key].freeze.position[2] = position[2] + game.engageplayerpos[2];

			} else {
				body[body_key].position[0] = position[0];
				body[body_key].position[1] = position[1];
				body[body_key].position[2] = position[2];
				body[body_key].freeze.position[0] = position[0];
				body[body_key].freeze.position[1] = position[1];
				body[body_key].freeze.position[2] = position[2];
			}
		} break;
		case JointContext: {
			if (game.engageplayerpos) {
				joint[joint_key].position[0] = position[0] + game.engageplayerpos[0];
				joint[joint_key].position[1] = position[1] + game.engageplayerpos[1];
				joint[joint_key].position[2] = position[2] + game.engageplayerpos[2];
				joint[joint_key].freeze.position[0] = position[0] + game.engageplayerpos[0];
				joint[joint_key].freeze.position[1] = position[1] + game.engageplayerpos[1];
				joint[joint_key].freeze.position[2] = position[2] + game.engageplayerpos[2];
			} else {
				joint[joint_key].position[0] = position[0];
				joint[joint_key].position[1] = position[1];
				joint[joint_key].position[2] = position[2];
				joint[joint_key].freeze.position[0] = position[0];
				joint[joint_key].freeze.position[1] = position[1];
				joint[joint_key].freeze.position[2] = position[2];
			}
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int orientation(lua_State* L)
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

	switch(DataContext)
	{
		case NoContext: {
			// Error Handling
		} break;
		case objectContext: {
			object[object_key].orientation[0] = orientation[0];
			object[object_key].orientation[1] = orientation[1];
			object[object_key].orientation[2] = orientation[2];
			object[object_key].orientation[3] = orientation[3];

			object[object_key].freeze.orientation[0] = orientation[0];
			object[object_key].freeze.orientation[1] = orientation[1];
			object[object_key].freeze.orientation[2] = orientation[2];
			object[object_key].freeze.orientation[3] = orientation[3];
		} break;
		case BodyContext: {
			body[body_key].orientation[0] = orientation[0];
			body[body_key].orientation[1] = orientation[1];
			body[body_key].orientation[2] = orientation[2];
			body[body_key].orientation[3] = orientation[3];

			body[body_key].freeze.orientation[0] = orientation[0];
			body[body_key].freeze.orientation[1] = orientation[1];
			body[body_key].freeze.orientation[2] = orientation[2];
			body[body_key].freeze.orientation[3] = orientation[3];
		} break;
		case JointContext: {
			joint[joint_key].orientation[0] = orientation[0];
			joint[joint_key].orientation[1] = orientation[1];
			joint[joint_key].orientation[2] = orientation[2];
			joint[joint_key].orientation[3] = orientation[3];
	
			joint[joint_key].freeze.orientation[0] = orientation[0];
			joint[joint_key].freeze.orientation[1] = orientation[1];
			joint[joint_key].freeze.orientation[2] = orientation[2];
			joint[joint_key].freeze.orientation[3] = orientation[3];
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int sides(lua_State* L)
{
	lua_Number sides[3];
		
	lua_rawgeti(L, -1, 1);
	sides[0] = lua_tonumber(L, -1); 

	lua_rawgeti(L, -2, 2);
	sides[1] = lua_tonumber(L, -1); 

	lua_rawgeti(L, -3, 3);
	sides[2] = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case NoContext: {
			// Error Handling
		} break;
		case objectContext: {
			object[object_key].sides[0] = sides[0];
			object[object_key].sides[1] = sides[1];
			object[object_key].sides[2] = sides[2];
		} break;
		case BodyContext: {
			body[body_key].sides[0] = sides[0];
			body[body_key].sides[1] = sides[1];
			body[body_key].sides[2] = sides[2];
		} break;
		case JointContext: {
			joint[joint_key].sides[0] = sides[0];
			joint[joint_key].sides[1] = sides[1];
			joint[joint_key].sides[2] = sides[2];
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

	switch(DataContext)
	{
		case NoContext: {
			// Error Handling
		} break;
		case objectContext: {
			object[object_key].density = density;
		} break;
		case BodyContext: {
			body[body_key].density = density;
		} break;
		case JointContext: {
			joint[joint_key].density = density;
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int api_static_state(lua_State* L)
{
	switch(DataContext)
	{
		case NoContext: {
			// Error Handling
		} break;
		case objectContext: {
			object[object_key].static_state = true;
		} break;
		case BodyContext: {
			body[body_key].static_state = true;
		} break;
		case JointContext: {
			joint[joint_key].static_state = true;
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int radius(lua_State* L)
{
	lua_rawgeti(L, -1, 1);

	lua_Number radius = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case NoContext: {
			// Error Handling
		} break;
		case objectContext: {
			object[object_key].radius = radius;
		} break;
		case BodyContext: {
			body[body_key].radius = radius;
		} break;
		case JointContext: {
			joint[joint_key].radius = radius;
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int length(lua_State* L)
{
	lua_rawgeti(L, -1, 1);

	lua_Number length = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case NoContext: {
			// Error Handling
		} break;
		case objectContext: {
			object[object_key].length = length;
		} break;
		case BodyContext: {
			body[body_key].length = length;
		} break;
		case JointContext: {
			joint[joint_key].length = length;
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int composite(lua_State* L)
{
	std::string name = lua_tostring(L, -1);
	
	switch(DataContext)
	{
		case NoContext: {
			// Error Handling
		} break;
		case objectContext: {
			object[object_key].isComposite = true;
			object[object_key].composite.name = name;
		} break;
		case BodyContext: {
			body[body_key].isComposite = true;
			body[body_key].composite.name = name;
		} break;
		case JointContext: {
			joint[joint_key].isComposite = true;
			joint[joint_key].composite.name = name;
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int strength(lua_State* L)
{
	lua_Number strength;
		
	lua_rawgeti(L, -1, 1);
	strength = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case JointContext: {
			joint[joint_key].strength = strength;
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int strength_alt(lua_State* L)
{
	lua_Number strength_alt;
		
	lua_rawgeti(L, -1, 1);
	strength_alt = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case JointContext: {
			joint[joint_key].strength_alt = strength_alt;
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}


int velocity(lua_State* L)
{
	lua_Number velocity;
		
	lua_rawgeti(L, -1, 1);
	velocity = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case JointContext: {
			joint[joint_key].velocity = velocity;
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int velocity_alt(lua_State* L)
{
	lua_Number velocity_alt;
		
	lua_rawgeti(L, -1, 1);
	velocity_alt = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case JointContext: {
			joint[joint_key].velocity_alt = velocity_alt;
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int axis(lua_State* L)
{
	lua_Number axis[3];
		
	lua_rawgeti(L, -1, 1);
	axis[0] = lua_tonumber(L, -1); 

	lua_rawgeti(L, -2, 2);
	axis[1] = lua_tonumber(L, -1); 

	lua_rawgeti(L, -3, 3);
	axis[2] = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case JointContext: {
			joint[joint_key].axis[0] = axis[0];
			joint[joint_key].axis[1] = axis[1];
			joint[joint_key].axis[2] = axis[2];
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int axis_alt(lua_State* L)
{
	lua_Number axis_alt[3];
		
	lua_rawgeti(L, -1, 1);
	axis_alt[0] = lua_tonumber(L, -1); 

	lua_rawgeti(L, -2, 2);
	axis_alt[1] = lua_tonumber(L, -1); 

	lua_rawgeti(L, -3, 3);
	axis_alt[2] = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case JointContext: {
			joint[joint_key].axis_alt[0] = axis_alt[0];
			joint[joint_key].axis_alt[1] = axis_alt[1];
			joint[joint_key].axis_alt[2] = axis_alt[2];
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int range(lua_State* L)
{
	lua_Number range[2];
		
	lua_rawgeti(L, -1, 1);
	range[0] = lua_tonumber(L, -1); 

	lua_rawgeti(L, -2, 2);
	range[1] = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case JointContext: {
			joint[joint_key].range[0] = range[0];
			joint[joint_key].range[1] = range[1];
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int range_alt(lua_State* L)
{
	lua_Number range_alt[2];
		
	lua_rawgeti(L, -1, 1);
	range_alt[0] = lua_tonumber(L, -1); 

	lua_rawgeti(L, -2, 2);
	range_alt[1] = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case JointContext: {
			joint[joint_key].range_alt[0] = range_alt[0];
			joint[joint_key].range_alt[1] = range_alt[1];
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int connections(lua_State* L)
{
	std::string connections[2];
		
	lua_rawgeti(L, -1, 1);
	connections[0] = lua_tostring(L, -1); 

	lua_rawgeti(L, -2, 2);
	connections[1] = lua_tostring(L, -1); 

	switch(DataContext)
	{
		case JointContext: {
			joint[joint_key].connections[0] = connections[0];
			joint[joint_key].connections[1] = connections[1];
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int connectionType(lua_State* L)
{
	std::string connectionType = lua_tostring(L, -1); 	

	switch(DataContext)
	{
		case JointContext: {
			if ("hinge" == connectionType) {
				joint[joint_key].connectionType = Hinge;
			} else if ("slider" == connectionType) {
				joint[joint_key].connectionType = Slider;
			} else if ("universal" == connectionType) {
				joint[joint_key].connectionType = Universal;
			} else if ("hinge2" == connectionType) {
				joint[joint_key].connectionType = Hinge2;
			} else {
			// Error Handling
			}
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

void init_api()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	
	lua_pushcfunction(L, api_turnframes);
	lua_setglobal(L, "turnframes");

	lua_pushcfunction(L, engageplayerpos);
	lua_setglobal(L, "engageplayerpos");

	lua_pushcfunction(L, engageplayerrot);
	lua_setglobal(L, "engageplayerrot");

	lua_pushcfunction(L, api_friction);
	lua_setglobal(L, "friction");

	lua_pushcfunction(L, gravity);
	lua_setglobal(L, "gravity");

	lua_pushcfunction(L, globalplane);
	lua_setglobal(L, "globalplane");

	lua_pushcfunction(L, api_object);
	lua_setglobal(L, "object");

	lua_pushcfunction(L, api_body);
	lua_setglobal(L, "body");

	lua_pushcfunction(L, api_joint);
	lua_setglobal(L, "joint");

	lua_pushcfunction(L, shape);
	lua_setglobal(L, "shape");

	lua_pushcfunction(L, position);
	lua_setglobal(L, "position");

	lua_pushcfunction(L, orientation);
	lua_setglobal(L, "orientation");

	lua_pushcfunction(L, sides);
	lua_setglobal(L, "sides");

	lua_pushcfunction(L, density);
	lua_setglobal(L, "density");

	lua_pushcfunction(L, api_static_state);
	lua_setglobal(L, "static");

	lua_pushcfunction(L, radius);
	lua_setglobal(L, "radius");

	lua_pushcfunction(L, length);
	lua_setglobal(L, "length");

	lua_pushcfunction(L, composite);
	lua_setglobal(L, "composite");

	lua_pushcfunction(L, strength);
	lua_setglobal(L, "strength");

	lua_pushcfunction(L, strength_alt);
	lua_setglobal(L, "strength_alt");

	lua_pushcfunction(L, velocity);
	lua_setglobal(L, "velocity");

	lua_pushcfunction(L, velocity_alt);
	lua_setglobal(L, "velocity_alt");

	lua_pushcfunction(L, axis);
	lua_setglobal(L, "axis");

	lua_pushcfunction(L, axis_alt);
	lua_setglobal(L, "axis_alt");

	lua_pushcfunction(L, axis);
	lua_setglobal(L, "axis");

	lua_pushcfunction(L, axis_alt);
	lua_setglobal(L, "axis_alt");

	lua_pushcfunction(L, range);
	lua_setglobal(L, "range");

	lua_pushcfunction(L, range_alt);
	lua_setglobal(L, "range_alt");

	lua_pushcfunction(L, connections);
	lua_setglobal(L, "connections");

	lua_pushcfunction(L, connectionType);
	lua_setglobal(L, "connectionType");

	if (luaL_dofile(L, "game.lua") != LUA_OK)
	{
		MSG = "[C] Error reading script";
		luaL_error(L, "Error: %s\n", lua_tostring(L, -1));
	} else MSG = "[C] Executed game.lua";	

	lua_close(L);
}

void GameStart()
{
	init_api();

	dMass mass;

	dInitODE2(0);

	game.gameframe = 0;
	game.freeze = true;
	game.freeze_time = 40;
	game.freeze_t = 0;
	game.unfreeze_time = 0;
	game.unfreeze_t = 0;

	game.world = dWorldCreate();
	game.space = dHashSpaceCreate(0);
  	game.contactgroup = dJointGroupCreate(0);
	game.floor = dCreatePlane(game.space, 0, 0, 1, 0);

	dGeomSetCategoryBits(game.floor, StaticobjectsCategoryBits);
	dGeomSetCollideBits(game.floor, StaticobjectsCollideBits);

  	dWorldSetGravity(game.world, game.gravity[0], game.gravity[1], game.gravity[2]);
	
	for (auto& [object_name, o] : object) {
		o.dBody = dBodyCreate(game.world);
		dBodySetPosition(
			o.dBody,
			o.position[0],
			o.position[1],
			o.position[2]
		);

		dBodySetQuaternion(o.dBody, o.orientation);
		
		switch(o.shape)
		{
			case Box: {
				o.dGeom = dCreateBox(game.space, o.sides[0], o.sides[1], o.sides[2]);
				dMassSetBox(&mass, o.density, o.sides[0], o.sides[1], o.sides[2]);
			} break;
			case Sphere: {
				o.dGeom = dCreateSphere(game.space, o.radius);
				dMassSetSphere(&mass, o.density, o.radius);
			} break;
			case Capsule: {
				o.dGeom = dCreateCapsule(game.space, o.radius, o.length);
				dMassSetCapsule(&mass, o.density, 1, o.length, o.radius);
			} break;
			case Cylinder: {
				o.dGeom = dCreateCylinder(game.space, o.radius, o.length);
				dMassSetCylinder(&mass, o.density, 1, o.length, o.radius);
			} break;
		}

		dMassAdjust(&mass, 0.5);
		dBodySetMass(o.dBody, &mass);
		dGeomSetBody(o.dGeom, o.dBody);

		if (o.static_state) {
			dJointID fixed = dJointCreateFixed(game.world ,0);
			dJointAttach(fixed, o.dBody, 0);
			dJointSetFixed(fixed);
			dGeomSetCategoryBits(o.dGeom, StaticobjectsCategoryBits);
			dGeomSetCollideBits(o.dGeom, StaticobjectsCollideBits);
		} else {
			dGeomSetCategoryBits(o.dGeom, DynamicobjectsCategoryBits);
			dGeomSetCollideBits(o.dGeom, DynamicobjectsCollideBits);
		}
	}

	for (auto& [body_name, b] : body) {
		b.dBody = dBodyCreate(game.world);
		dBodySetPosition(
			b.dBody,
			b.position[0],
			b.position[1],
			b.position[2]
		);

		dBodySetQuaternion(b.dBody, b.orientation);

		switch(b.shape)
		{
			case Box: {
				b.dGeom = dCreateBox(game.space, b.sides[0], b.sides[1], b.sides[2]);
				dMassSetBox(&mass, b.density, b.sides[0], b.sides[1], b.sides[2]);
			} break;
			case Sphere: {
				b.dGeom = dCreateSphere(game.space, b.radius);
				dMassSetSphere(&mass, b.density, b.radius);
			} break;
			case Capsule: {
				b.dGeom = dCreateCapsule(game.space, b.radius, b.length);
				dMassSetCapsule(&mass, b.density, 1, b.radius, b.length);
			} break;
			case Cylinder: {
				b.dGeom = dCreateCylinder(game.space, b.radius, b.length);
				dMassSetCylinder(&mass, b.density, 1, b.radius, b.length);
			} break;
		}

		dMassAdjust(&mass, 0.5);
		dBodySetMass(b.dBody, &mass);
		dGeomSetBody(b.dGeom, b.dBody);

		if (b.static_state) {
			dJointID fixed = dJointCreateFixed(game.world ,0);
			dJointAttach(fixed, b.dBody, 0);
			dJointSetFixed(fixed);
		} else {
		}

		dGeomSetCategoryBits(b.dGeom, BodyCategoryBits);
		dGeomSetCollideBits(b.dGeom, BodyCollideBits);
	}

	for (auto& [joint_name, j] : joint) {
		j.dBody = dBodyCreate(game.world);
		dBodySetPosition(
			j.dBody,
			j.position[0],
			j.position[1],
			j.position[2]
		);

		dBodySetQuaternion(j.dBody, j.orientation);

		switch(j.shape)
		{
			case Box: {
				j.dGeom = dCreateBox(game.space, j.sides[0], j.sides[1], j.sides[2]);
				dMassSetBox(&mass, j.density, j.sides[0], j.sides[1], j.sides[2]);
			} break;
			case Sphere: {
				j.dGeom = dCreateSphere(game.space, j.radius);
				dMassSetSphere(&mass, j.density, j.radius);
			} break;
			case Capsule: {
				j.dGeom = dCreateCapsule(game.space, j.radius, j.length);
				dMassSetCapsule(&mass, j.density, 1, j.radius, j.length);
			} break;
			case Cylinder: {
				j.dGeom = dCreateCylinder(game.space, j.radius, j.length);
				dMassSetCylinder(&mass, j.density, 1, j.radius, j.length);
			} break;
		}

		dMassAdjust(&mass, 0.5);
		dBodySetMass(j.dBody, &mass);
		dGeomSetBody(j.dGeom, j.dBody);

		if (j.static_state) {
			dJointID fixed = dJointCreateFixed(game.world ,0);
			dJointAttach(fixed, j.dBody, 0);
			dJointSetFixed(fixed);
		}
		switch(j.connectionType)
		{
			case Hinge: {
				j.dJoint[0] = dJointCreateHinge(game.world, 0);
				dJointAttach(
					j.dJoint[0],
					body[j.connections[0]].dBody,
					j.dBody);
				dJointSetHingeAnchor(
					j.dJoint[0],
					j.position[0],
					j.position[1],
					j.position[2]);
				dJointSetHingeAxis(
					j.dJoint[0],
					j.axis[0],
					j.axis[1],
					j.axis[2]);
		
				dJointSetHingeParam(
					j.dJoint[0],
					dParamHiStop,
					j.range[0]);
				dJointSetHingeParam(
					j.dJoint[0],
					dParamLoStop,
					j.range[1]);
		
				j.dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(
					j.dJoint[1],
					j.dBody,
					body[j.connections[1]].dBody);
				dJointSetFixed(j.dJoint[1]);
			} break;
			case Slider: {
				j.dJoint[0] = dJointCreateSlider(game.world, 0);
				dJointAttach(
					j.dJoint[0],
					body[j.connections[0]].dBody,
					j.dBody);
				dJointSetSliderAxis(
					j.dJoint[0],
					j.axis[0],
					j.axis[1],
					j.axis[2]);
		
				dJointSetSliderParam(
					j.dJoint[0],
					dParamHiStop,
					j.range[0]);
				dJointSetSliderParam(
					j.dJoint[0],
					dParamLoStop,
					j.range[1]);
		
				j.dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(
					j.dJoint[1],
					j.dBody,
					body[j.connections[1]].dBody);
				dJointSetFixed(j.dJoint[1]);
			} break;
			case Universal: {
				j.dJoint[0] = dJointCreateUniversal(game.world, 0);
				dJointAttach(
					j.dJoint[0],
					body[j.connections[0]].dBody,
					j.dBody);
				dJointSetUniversalAnchor(
					j.dJoint[0],
					j.position[0],
					j.position[1],
					j.position[2]);

				dJointSetUniversalAnchor(
					j.dJoint[0],
					j.position[0],
					j.position[1],
					j.position[2]);

				dJointSetUniversalAxis1(
					j.dJoint[0],
					j.axis[0],
					j.axis[1],
					j.axis[2]);
				dJointSetUniversalAxis2(
					j.dJoint[0],
					j.axis_alt[0],
					j.axis_alt[1],
					j.axis_alt[2]);
		
				dJointSetUniversalParam(
					j.dJoint[0],
					dParamHiStop,
					j.range[0]);

				dJointSetUniversalParam(
					j.dJoint[0],
					dParamHiStop2,
					j.range_alt[0]);

				dJointSetUniversalParam(
					j.dJoint[0],
					dParamLoStop,
					j.range[1]);

				dJointSetUniversalParam(
					j.dJoint[0],
					dParamLoStop2,
					j.range_alt[1]);
		
				j.dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(
					j.dJoint[1],
					j.dBody,
					body[j.connections[1]].dBody);
				dJointSetFixed(j.dJoint[1]);
			} break;
			case Hinge2: {
				j.dJoint[0] = dJointCreateHinge2(game.world, 0);
				dJointAttach(
					j.dJoint[0],
					body[j.connections[0]].dBody,
					j.dBody);
				dJointSetHinge2Anchor(
					j.dJoint[0],
					j.position[0],
					j.position[1],
					j.position[2]);

				dJointSetHinge2Anchor(
					j.dJoint[0],
					j.position[0],
					j.position[1],
					j.position[2]);
			
				dJointSetHinge2Axes(j.dJoint[0], j.axis, j.axis_alt);

				dJointSetHinge2Param(
					j.dJoint[0],
					dParamHiStop,
					j.range[0]);

				dJointSetHinge2Param(
					j.dJoint[0],
					dParamHiStop2,
					j.range_alt[0]);

				dJointSetHinge2Param(
					j.dJoint[0],
					dParamLoStop,
					j.range[1]);

				dJointSetHinge2Param(
					j.dJoint[0],
					dParamLoStop2,
					j.range_alt[1]);
		
				j.dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(
					j.dJoint[1],
					j.dBody,
					body[j.connections[1]].dBody);
				dJointSetFixed(j.dJoint[1]);
			} break;
			default:
				j.dJoint[0] = dJointCreateFixed(game.world, 0);
				dJointAttach(
					j.dJoint[0],
					body[j.connections[0]].dBody,
					j.dBody);
				dJointSetFixed(j.dJoint[0]);

				j.dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(
					j.dJoint[1],
					j.dBody,
					body[j.connections[1]].dBody);
				dJointSetFixed(j.dJoint[1]);
		}

		dGeomSetCategoryBits(j.dGeom, JointCategoryBits);
		dGeomSetCollideBits(j.dGeom, JointCollideBits);
	}
}

void UpdateFreeze()
{
	game.freeze = true;
	game.unfreeze_t = 0;

	for (auto& [obejct_name, o] : object) {
		const dReal *position = dBodyGetPosition(o.dBody);
		const dReal *orientation = dBodyGetQuaternion(o.dBody);
		const dReal *linearVel = dBodyGetLinearVel(o.dBody);
		const dReal *angularVel = dBodyGetAngularVel(o.dBody);

		o.freeze.position[0] = position[0];
		o.freeze.position[1] = position[1];
		o.freeze.position[2] = position[2];

		o.freeze.orientation[0] = orientation[0];
		o.freeze.orientation[1] = orientation[1];
		o.freeze.orientation[2] = orientation[2];
		o.freeze.orientation[3] = orientation[3];

		o.freeze.linearVel[0] = linearVel[0];
		o.freeze.linearVel[1] = linearVel[1];
		o.freeze.linearVel[2] = linearVel[2];

		o.freeze.angularVel[0] = angularVel[0];
		o.freeze.angularVel[1] = angularVel[1];
		o.freeze.angularVel[2] = angularVel[2];
	}

	for (auto& [body_name, b] : body) {
		const dReal *position = dBodyGetPosition(b.dBody);
		const dReal *orientation = dBodyGetQuaternion(b.dBody);
		const dReal *linearVel = dBodyGetLinearVel(b.dBody);
		const dReal *angularVel = dBodyGetAngularVel(b.dBody);

		b.freeze.position[0] = position[0];
		b.freeze.position[1] = position[1];
		b.freeze.position[2] = position[2];

		b.freeze.orientation[0] = orientation[0];
		b.freeze.orientation[1] = orientation[1];
		b.freeze.orientation[2] = orientation[2];
		b.freeze.orientation[3] = orientation[3];

		b.freeze.linearVel[0] = linearVel[0];
		b.freeze.linearVel[1] = linearVel[1];
		b.freeze.linearVel[2] = linearVel[2];

		b.freeze.angularVel[0] = angularVel[0];
		b.freeze.angularVel[1] = angularVel[1];
		b.freeze.angularVel[2] = angularVel[2];
	}
	
	for (auto& [joint_name, j] : joint) {
		const dReal *position = dBodyGetPosition(j.dBody);
		const dReal *orientation = dBodyGetQuaternion(j.dBody);
		const dReal *linearVel = dBodyGetLinearVel(j.dBody);
		const dReal *angularVel = dBodyGetAngularVel(j.dBody);

		j.freeze.position[0] = position[0];
		j.freeze.position[1] = position[1];
		j.freeze.position[2] = position[2];

		j.freeze.orientation[0] = orientation[0];
		j.freeze.orientation[1] = orientation[1];
		j.freeze.orientation[2] = orientation[2];
		j.freeze.orientation[3] = orientation[3];

		j.freeze.linearVel[0] = linearVel[0];
		j.freeze.linearVel[1] = linearVel[1];
		j.freeze.linearVel[2] = linearVel[2];

		j.freeze.angularVel[0] = angularVel[0];
		j.freeze.angularVel[1] = angularVel[1];
		j.freeze.angularVel[2] = angularVel[2];
	}
}

void DrawSim()
{
		for (auto const& [object_name, o] : object) {
			dQuaternion dQ;
			dRtoQ(dBodyGetRotation(o.dBody), dQ);

			float angle;
			Vector3 axis;
			Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
			QuaternionToAxisAngle(q, &axis, &angle);
			const dReal *pos = dBodyGetPosition(o.dBody);
			rlPushMatrix();
			rlTranslatef(pos[0], pos[1], pos[2]);
			rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

			switch(o.shape)
			{
				case Box: {
					if (o.static_state)
						DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, o.sides[0], o.sides[1], o.sides[2], StaticobjectColor);
					else
						DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, o.sides[0], o.sides[1], o.sides[2], DynamicobjectColor);
					DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, o.sides[0], o.sides[1], o.sides[2], BLACK);
				} break;
				case Sphere: {
					if (o.static_state)
						DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, o.radius, StaticobjectColor);
					else
						DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, o.radius, DynamicobjectColor);
					DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, o.radius, 16, 16, BLACK);
				} break;
				case Capsule: {
					if (o.static_state)
						DrawCapsule((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, 16, 16, StaticobjectColor);
					else
						DrawCapsule((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, 16, 16, DynamicobjectColor);
					DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, 16, 16, BLACK);
				} break;
				case Cylinder: {
					if (o.static_state)
						DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, o.radius, 16, StaticobjectColor);
					else
						DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, o.radius, 16, DynamicobjectColor);
					DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, o.radius, 16, BLACK);
				} break;
			}

			rlPopMatrix();
		}
	for (auto const& [body_name, b] : body)	 {
			dQuaternion dQ;
			dRtoQ(dBodyGetRotation(b.dBody), dQ);
			float angle;
			Vector3 axis;
			Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
			QuaternionToAxisAngle(q, &axis, &angle);
			const dReal *pos = dBodyGetPosition(b.dBody);
			rlPushMatrix();
			rlTranslatef(pos[0], pos[1], pos[2]);
			rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

			switch(b.shape) {
			case Box: {
				DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, b.sides[0], b.sides[1], b.sides[2], BodyColor);
				DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, b.sides[0], b.sides[1], b.sides[2], BLACK);
			} break;
			case Sphere: {
				DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, b.radius, BodyColor);
				DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, b.radius, 16, 16, BLACK);
			} break;
			case Capsule: {
				DrawCapsule((Vector3){ 0.0f, 0.0f, -(b.length/2) }, (Vector3){ 0.0f, 0.0f, (b.length/2) }, b.radius, 16, 16, BodyColor);
				DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(b.length/2) }, (Vector3){ 0.0f, 0.0f, (b.length/2) }, b.radius, 16, 16, BLACK);
			} break;
			case Cylinder: {
				DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(b.length/2) }, (Vector3){ 0.0f, 0.0f, (b.length/2) }, b.radius, b.radius, 16, BodyColor);
				DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(b.length/2) }, (Vector3){ 0.0f, 0.0f, (b.length/2) }, b.radius, b.radius, 16, BLACK);
			} break;
		}
		rlPopMatrix();
	}

	for (auto const& [joint_name, j] : joint) {
		dQuaternion dQ;
		dRtoQ(dBodyGetRotation(j.dBody), dQ);

		float angle;
		Vector3 axis;
		Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
		QuaternionToAxisAngle(q, &axis, &angle);
		const dReal *pos = dBodyGetPosition(j.dBody);
		rlPushMatrix();
		rlTranslatef(pos[0], pos[1], pos[2]);
		rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

		switch(j.shape) {
			case Box: {
				DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, j.sides[0], j.sides[1], j.sides[2], JointColor);
				DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, j.sides[0], j.sides[1], j.sides[2], BLACK);
			} break;
			case Sphere: {
				DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, j.radius, JointColor);
				DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, j.radius, 16, 16, BLACK);
			} break;
			case Capsule: {
				DrawCapsule((Vector3){ 0.0f, 0.0f, -(j.length/2) }, (Vector3){ 0.0f, 0.0f, (j.length/2) }, j.radius, 16, 16, JointColor);
				DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(j.length/2) }, (Vector3){ 0.0f, 0.0f, (j.length/2) }, j.radius, 16, 16, BLACK);
			} break;
			case Cylinder: {
				DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(j.length/2) }, (Vector3){ 0.0f, 0.0f, (j.length/2) }, j.radius, j.radius, 16, JointColor);
				DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(j.length/2) }, (Vector3){ 0.0f, 0.0f, (j.length/2) }, j.radius, j.radius, 16, BLACK);
			} break;
		}
		rlPopMatrix();
	}
}

void DrawGhost()
{
	for (auto const& [object_name, o] : object) {
		dQuaternion dQ;
		dRtoQ(dBodyGetRotation(o.dBody), dQ);

		float angle;
		Vector3 axis;
		Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
		QuaternionToAxisAngle(q, &axis, &angle);
		const dReal *pos = dBodyGetPosition(o.dBody);
		rlPushMatrix();
		rlTranslatef(pos[0], pos[1], pos[2]);
		rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

		switch(o.shape) {
			case Box: {
				DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, o.sides[0], o.sides[1], o.sides[2], GhostColor);
				DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, o.sides[0], o.sides[1], o.sides[2], BLACK);
			} break;
			case Sphere: {
				DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, o.radius, GhostColor);
				DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, o.radius, 16, 16, BLACK);
			} break;
			case Capsule: {
				DrawCapsule((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, 16, 16, GhostColor);
				DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, 16, 16, BLACK);
			} break;
			case Cylinder: {
				DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, o.radius, 16, GhostColor);
				DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, o.radius, 16, BLACK);
			} break;
		}
		rlPopMatrix();
	}
		
	for (auto const& [body_name, b] : body) {
		dQuaternion dQ;
		dRtoQ(dBodyGetRotation(b.dBody), dQ);

		float angle;
		Vector3 axis;
		Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
		QuaternionToAxisAngle(q, &axis, &angle);
		const dReal *pos = dBodyGetPosition(b.dBody);
		rlPushMatrix();
		rlTranslatef(pos[0], pos[1], pos[2]);
		rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

		switch(b.shape) {
			case Box: {
				DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, b.sides[0], b.sides[1], b.sides[2], GhostColor);
				DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, b.sides[0], b.sides[1], b.sides[2], BLACK);
			} break;
			case Sphere: {
				DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, b.radius, GhostColor);
				DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, b.radius, 16, 16, BLACK);
			} break;
			case Capsule: {
					DrawCapsule((Vector3){ 0.0f, 0.0f, -(b.length/2) }, (Vector3){ 0.0f, 0.0f, (b.length/2) }, b.radius, 16, 16, GhostColor);
					DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(b.length/2) }, (Vector3){ 0.0f, 0.0f, (b.length/2) }, b.radius, 16, 16, BLACK);
			} break;
			case Cylinder: {
				DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(b.length/2) }, (Vector3){ 0.0f, 0.0f, (b.length/2) }, b.radius, b.radius, 16, GhostColor);
				DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(b.length/2) }, (Vector3){ 0.0f, 0.0f, (b.length/2) }, b.radius, b.radius, 16, BLACK);
			} break;
		}
		rlPopMatrix();
	}

	for (auto const& [joint_name, j] : joint) {
		dQuaternion dQ;
		dRtoQ(dBodyGetRotation(j.dBody), dQ);

		float angle;
		Vector3 axis;
		Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
		QuaternionToAxisAngle(q, &axis, &angle);
		const dReal *pos = dBodyGetPosition(j.dBody);
		rlPushMatrix();
		rlTranslatef(pos[0], pos[1], pos[2]);
		rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

		switch(j.shape)
		{
			case Box: {
				DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, j.sides[0], j.sides[1], j.sides[2], GhostColor);
				DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, j.sides[0], j.sides[1], j.sides[2], BLACK);
			} break;
			case Sphere: {
				DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, j.radius, GhostColor);
				DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, j.radius, 16, 16, BLACK);
			} break;
			case Capsule: {
				// WTF is with the capsules hitbox
				DrawCapsule((Vector3){ 0.0f, 0.0f, -(j.length/2) }, (Vector3){ 0.0f, 0.0f, (j.length/2) }, j.radius, 16, 16, GhostColor);
				DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(j.length/2) }, (Vector3){ 0.0f, 0.0f, (j.length/2) }, j.radius, 16, 16, BLACK);
			} break;
			case Cylinder: {
				DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(j.length/2) }, (Vector3){ 0.0f, 0.0f, (j.length/2) }, j.radius, j.radius, 16, GhostColor);
				DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(j.length/2) }, (Vector3){ 0.0f, 0.0f, (j.length/2) }, j.radius, j.radius, 16, BLACK);
			} break;
		}
		rlPopMatrix();
	}
}

void DrawFreeze()
{
	for (auto const& [object_name, o] : object) {
		float angle;
		Vector3 axis;
		Quaternion q = {
			o.freeze.orientation[1],
			o.freeze.orientation[2],
			o.freeze.orientation[3],
			o.freeze.orientation[0]
		};
		QuaternionToAxisAngle(q, &axis, &angle);
		rlPushMatrix();
		rlTranslatef(
			o.freeze.position[0],
			o.freeze.position[1],
			o.freeze.position[2]
		);
		rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

		switch(o.shape) {
			case Box: {
				if (o.static_state) {
					DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, o.sides[0], o.sides[1], o.sides[2], StaticobjectColor);
				} else {
					DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, o.sides[0], o.sides[1], o.sides[2], DynamicobjectColor);
				}
				DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, o.sides[0], o.sides[1], o.sides[2], BLACK);
				} break;
			case Sphere: {
				if (o.static_state) {
					DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, o.radius, StaticobjectColor);
				} else {
					DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, o.radius, DynamicobjectColor);
				}
				DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, o.radius, 16, 16, BLACK);
				} break;
			case Capsule: {
				if (o.static_state) {
					DrawCapsule((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, 16, 16, StaticobjectColor);
				} else {
					DrawCapsule((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, 16, 16, DynamicobjectColor);
				}
				DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, 16, 16, BLACK);
				} break;
			case Cylinder: {
				if (o.static_state) {
					DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, o.radius, 16, StaticobjectColor);
				} else {
					DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, o.radius, 16, DynamicobjectColor);
				}
				DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(o.length/2) }, (Vector3){ 0.0f, 0.0f, (o.length/2) }, o.radius, o.radius, 16, BLACK);
				} break;
			}
			rlPopMatrix();
		}
		
	for (auto const& [body_name, b] : body) {
		float angle;
		Vector3 axis;
		Quaternion q = {
			b.freeze.orientation[1],
			b.freeze.orientation[2],
			b.freeze.orientation[3],
			b.freeze.orientation[0]
		};
		QuaternionToAxisAngle(q, &axis, &angle);
		rlPushMatrix();
		rlTranslatef(
			b.freeze.position[0],
			b.freeze.position[1],
			b.freeze.position[2]
		);
		rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

		switch (b.shape) {
			case Box: {
				DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, b.sides[0], b.sides[1], b.sides[2], BodyColor);
				DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, b.sides[0], b.sides[1], b.sides[2], BLACK);
			} break;
			case Sphere: {
				DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, b.radius, BodyColor);
				DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, b.radius, 16, 16, BLACK);
			} break;
			case Capsule: {
				DrawCapsule((Vector3){ 0.0f, 0.0f, -(b.length/2) }, (Vector3){ 0.0f, 0.0f, (b.length/2) }, b.radius, 16, 16, BodyColor);
				DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(b.length/2) }, (Vector3){ 0.0f, 0.0f, (b.length/2) }, b.radius, 16, 16, BLACK);
			} break;
			case Cylinder: {
				DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(b.length/2) }, (Vector3){ 0.0f, 0.0f, (b.length/2) }, b.radius, b.radius, 16, BodyColor);
				DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(b.length/2) }, (Vector3){ 0.0f, 0.0f, (b.length/2) }, b.radius, b.radius, 16, BLACK);
			} break;
		}
		rlPopMatrix();
	}

	for (auto const& [joint_name, j] : joint) {
		float angle;
		Vector3 axis;
		Quaternion q = {
			j.freeze.orientation[1],
			j.freeze.orientation[2],
			j.freeze.orientation[3],
			j.freeze.orientation[0]
		};
		QuaternionToAxisAngle(q, &axis, &angle);
		rlPushMatrix();
		rlTranslatef(
			j.freeze.position[0],
			j.freeze.position[1],
			j.freeze.position[2]
		);
		rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

		switch(j.shape)
		{
			case Box: {
				DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, j.sides[0], j.sides[1], j.sides[2], JointColor);
				DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, j.sides[0], j.sides[1], j.sides[2], BLACK);
			} break;
			case Sphere: {
				if (j.name == game.selected_joint)
					DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, j.radius, RED);
				else
					DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, j.radius, JointColor);
				DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, j.radius, 16, 16, BLACK);
			} break;
			case Capsule: {
				// WTF is with the capsules hitbox
				DrawCapsule((Vector3){ 0.0f, 0.0f, -(j.length/2) }, (Vector3){ 0.0f, 0.0f, (j.length/2) }, j.radius, 16, 16, JointColor);
				DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(j.length/2) }, (Vector3){ 0.0f, 0.0f, (j.length/2) }, j.radius, 16, 16, BLACK);
			} break;
			case Cylinder: {
				DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(j.length/2) }, (Vector3){ 0.0f, 0.0f, (j.length/2) }, j.radius, j.radius, 16, JointColor);
				DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(j.length/2) }, (Vector3){ 0.0f, 0.0f, (j.length/2) }, j.radius, j.radius, 16, BLACK);
			} break;
		}
		rlPopMatrix();
	}
}


void ReFreeze()
{
	game.freeze_t = 0;

	for (auto const& [object_name, o] : object) {
		dBodySetPosition(
			o.dBody,
			o.freeze.position[0],
			o.freeze.position[1],
			o.freeze.position[2]);
		dBodySetQuaternion(o.dBody, o.freeze.orientation);
		dBodySetLinearVel(
			o.dBody,
			o.freeze.linearVel[0],
			o.freeze.linearVel[1],
			o.freeze.linearVel[2]);
		dBodySetAngularVel(
			o.dBody,
			o.freeze.angularVel[0],
			o.freeze.angularVel[1],
			o.freeze.angularVel[2]);
	}

	for (auto const& [body_name, b] : body) {
		dBodySetPosition(
			b.dBody,
			b.freeze.position[0],
			b.freeze.position[1],
			b.freeze.position[2]);
		dBodySetQuaternion(b.dBody, b.freeze.orientation);
		dBodySetLinearVel(
			b.dBody,
			b.freeze.linearVel[0],
			b.freeze.linearVel[1],
			b.freeze.linearVel[2]);
		dBodySetAngularVel(
			b.dBody,
			b.freeze.angularVel[0],
			b.freeze.angularVel[1],
			b.freeze.angularVel[2]);
	}

	for (auto const& [joint_name, j] : joint) {
		dBodySetPosition(
			j.dBody,
			j.freeze.position[0],
			j.freeze.position[1],
			j.freeze.position[2]);
		dBodySetQuaternion(j.dBody, j.freeze.orientation);
		dBodySetLinearVel(
			j.dBody,
			j.freeze.linearVel[0],
			j.freeze.linearVel[1],
			j.freeze.linearVel[2]);
		dBodySetAngularVel(
			j.dBody,
			j.freeze.angularVel[0],
			j.freeze.angularVel[1],
			j.freeze.angularVel[2]);
	}
}

void DrawFloor()
{
	rlPushMatrix();
	float angle;
	Vector3 axis;
	Matrix m = MatrixRotateX(DEG2RAD*90);
	Quaternion q = QuaternionFromMatrix(m);
	QuaternionToAxisAngle(q, &axis, &angle);
	rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);
	DrawGrid(20, 1.0f);
	rlPopMatrix();
}

void DrawFrame()
{
	DrawFloor();
	if (game.freeze) {
		DrawFreeze();
		DrawGhost();
	} else {
		DrawSim();
	}
}

void GameEnd()
{
	dJointGroupDestroy(game.contactgroup);
	dSpaceDestroy(game.space);
	dCloseODE();
}

void GameStep(int frame_count)
{
	game.unfreeze_time = frame_count;
	game.freeze = false;
	ReFreeze();
}

/* TODO: implement this per player
void TriggerPlayerPassiveStates(std::string player)
{

}
*/

void TriggerPlayerPassiveStates(PlayerPassiveStates state)
{
	dReal strength = 0.00;
	for (auto& [joint_name, j] : joint) {
		j.state = RELAX;
		if (state == HOLD_ALL) {
			strength = j.strength;
			j.state = HOLD;
		}
		switch(j.connectionType) {
			case Hinge: {
				dJointSetHingeParam(j.dJoint[0], dParamFMax, strength);
				dJointSetHingeParam(j.dJoint[0], dParamVel, 0.0f);
			} break;
			case Slider: {
				dJointSetSliderParam(j.dJoint[0], dParamFMax, strength);
				dJointSetSliderParam(j.dJoint[0], dParamVel, 0.0f);
			} break;
			case Universal: {
				dJointSetUniversalParam(j.dJoint[0], dParamFMax, strength);
				dJointSetUniversalParam(j.dJoint[0], dParamVel, 0.0f);
			} break;
			case Hinge2: {
				dJointSetHinge2Param(j.dJoint[0], dParamFMax, strength);
				dJointSetHinge2Param(j.dJoint[0], dParamVel, 0.0f);
			} break;
		}
	}
}

/* TODO: implement this per player
void TriggerPlayerPassiveStatesAlt(std::string player)
{

}
*/

void TriggerPlayerPassiveStatesAlt(PlayerPassiveStates state)
{
	dReal strength = 0.00;
	for (auto& [joint_name, j] : joint) {
		j.state_alt = RELAX;
		if (state == HOLD_ALL) {
			strength = j.strength_alt;
			j.state_alt = HOLD;
		}
		switch(j.connectionType) {
			case Hinge: {
			} break;
			case Slider: {
			} break;
			case Universal: {
				dJointSetUniversalParam(j.dJoint[0], dParamFMax2, strength);
				dJointSetUniversalParam(j.dJoint[0], dParamVel2, 0.0f);
			} break;
			case Hinge2: {
				dJointSetHinge2Param(j.dJoint[0], dParamFMax2, strength);
				dJointSetHinge2Param(j.dJoint[0], dParamVel2, 0.0f);
			} break;
		}
	}
}

/* TODO: implement this per player
void TogglePlayerPassiveStates(std::string player)
{

}
*/

void TogglePlayerPassiveStates()
{
	ReFreeze();
	if (player.passive_states == RELAX_ALL) {
		TriggerPlayerPassiveStates(HOLD_ALL);
		player.passive_states = HOLD_ALL;
	} else {
		TriggerPlayerPassiveStates(RELAX_ALL);
		player.passive_states = RELAX_ALL;
	}
}

/* TODO: implement this per player
void TogglePlayerPassiveStatesAlt(std::string player)
{

}
*/

void TogglePlayerPassiveStatesAlt()
{
	ReFreeze();
	if (player.passive_states_alt == RELAX_ALL) {
		TriggerPlayerPassiveStatesAlt(HOLD_ALL);
		player.passive_states_alt = HOLD_ALL;
	} else {
		TriggerPlayerPassiveStatesAlt(RELAX_ALL);
		player.passive_states_alt = RELAX_ALL;
	}
}

/*
 * TODO: implement this per player
void RelaxAll(std::string player)
{

}
*/

void RelaxAll()
{
	ReFreeze();
	TriggerPlayerPassiveStates(RELAX_ALL);
	player.passive_states = RELAX_ALL;
}

void RelaxAllAlt()
{
	ReFreeze();
	TriggerPlayerPassiveStatesAlt(RELAX_ALL);
	player.passive_states_alt = RELAX_ALL;
}

void TriggerActiveStateAlt(std::string joint_name, dReal direction)
{
	switch(joint[joint_name].connectionType) {
		case Hinge: {
		} break;
		case Slider: {
		} break;
		case Universal: {
			dJointSetUniversalParam(joint[joint_name].dJoint[0], dParamFMax2, joint[joint_name].strength_alt);
			dJointSetUniversalParam(joint[joint_name].dJoint[0], dParamVel2, direction * joint[joint_name].velocity_alt);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(joint[joint_name].dJoint[0], dParamFMax2, joint[joint_name].strength_alt);
			dJointSetHinge2Param(joint[joint_name].dJoint[0], dParamVel2, direction * joint[joint_name].velocity_alt);
		} break;
	}
}

void TriggerPassiveStateAlt(std::string joint_name, dReal strength)
{
	switch(joint[joint_name].connectionType)
	{
		case Hinge: {
		} break;
		case Slider: {
		} break;
		case Universal: {
			dJointSetUniversalParam(joint[joint_name].dJoint[0], dParamFMax2, strength);
			dJointSetUniversalParam(joint[joint_name].dJoint[0], dParamVel2, 0.0f);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(joint[joint_name].dJoint[0], dParamFMax2, strength);
			dJointSetHinge2Param(joint[joint_name].dJoint[0], dParamVel2, 0.0f);
		} break;
	}
}
void TriggerActiveState(std::string joint_name, dReal direction)
{
	switch(joint[joint_name].connectionType) {
		case Hinge: {
			dJointSetHingeParam(joint[joint_name].dJoint[0], dParamFMax, joint[joint_name].strength);
			dJointSetHingeParam(joint[joint_name].dJoint[0], dParamVel, direction * joint[joint_name].velocity);
		} break;
		case Slider: {
			dJointSetSliderParam(joint[joint_name].dJoint[0], dParamFMax, joint[joint_name].strength);
			dJointSetSliderParam(joint[joint_name].dJoint[0], dParamVel, direction * joint[joint_name].velocity);
		} break;
		case Universal: {
			dJointSetUniversalParam(joint[joint_name].dJoint[0], dParamFMax, joint[joint_name].strength);
			dJointSetUniversalParam(joint[joint_name].dJoint[0], dParamVel, direction * joint[joint_name].velocity);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(joint[joint_name].dJoint[0], dParamFMax, joint[joint_name].strength);
			dJointSetHinge2Param(joint[joint_name].dJoint[0], dParamVel, direction * joint[joint_name].velocity);
		} break;
	}
}

void TriggerPassiveState(std::string joint_name, dReal strength)
{
	switch(joint[joint_name].connectionType) {
		case Hinge: {
			dJointSetHingeParam(joint[joint_name].dJoint[0], dParamFMax, strength);
			dJointSetHingeParam(joint[joint_name].dJoint[0], dParamVel, 0.0f);
		} break;
		case Slider: {
			dJointSetSliderParam(joint[joint_name].dJoint[0], dParamFMax, strength);
			dJointSetSliderParam(joint[joint_name].dJoint[0], dParamVel, 0.0f);
		} break;
		case Universal: {
			dJointSetUniversalParam(joint[joint_name].dJoint[0], dParamFMax, strength);
			dJointSetUniversalParam(joint[joint_name].dJoint[0], dParamVel, 0.0f);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(joint[joint_name].dJoint[0], dParamFMax, strength);
			dJointSetHinge2Param(joint[joint_name].dJoint[0], dParamVel, 0.0f);
		} break;
	}
}

void TogglePassiveState(std::string joint_name)
{
	if (joint_name == "NONE") return;

	ReFreeze();

	if (joint[joint_name].state == RELAX) {
		joint[joint_name].state = HOLD;
		TriggerPassiveState(joint_name, joint[joint_name].strength);
	} else {
		joint[joint_name].state = RELAX;
		TriggerPassiveState(joint_name, 0);
	}
}

void TogglePassiveStateAlt(std::string joint_name)
{
	if (joint_name == "NONE") return;

	ReFreeze();

	if (joint[joint_name].state_alt == RELAX) {
		joint[joint_name].state_alt = HOLD;
		TriggerPassiveStateAlt(joint_name, joint[joint_name].strength);
	} else {
		joint[joint_name].state_alt = RELAX;
		TriggerPassiveStateAlt(joint_name, 0);
	}
}

void ToggleActiveState(std::string joint_name)
{
	if (joint_name == "NONE") return;

	ReFreeze();
	
	if (joint[joint_name].state == FORWARD) {
		joint[joint_name].state = BACKWARD;
		TriggerActiveState(joint_name, -1);
	} else {
		joint[joint_name].state = FORWARD;
		TriggerActiveState(joint_name, 1);
	}
}

void ToggleActiveStateAlt(std::string joint_name)
{
	if (joint_name == "NONE") return;

	ReFreeze();
	
	if (joint[joint_name].state_alt == FORWARD) {
		joint[joint_name].state_alt = BACKWARD;
		TriggerActiveStateAlt(joint_name, -1);
	} else {
		joint[joint_name].state_alt = FORWARD;
		TriggerActiveStateAlt(joint_name, 1);
	}
}


void CycleState(std::string joint_name)
{
	if (joint_name == "NONE") return;

	ReFreeze();
	
	if (joint[joint_name].state == FORWARD) {
		joint[joint_name].state = BACKWARD;
		TriggerActiveState(joint_name, -1);
	} else if (joint[joint_name].state == BACKWARD) {
		joint[joint_name].state = HOLD;
		TriggerPassiveState(joint_name, joint[joint_name].strength);
	} else if (joint[joint_name].state == HOLD) {
		joint[joint_name].state = RELAX;
		TriggerPassiveState(joint_name, 0);
	} else {
		joint[joint_name].state = FORWARD;
		TriggerActiveState(joint_name, 1);
	}
}


void CycleStateAlt(std::string joint_name)
{
	if (joint_name == "NONE") return;

	ReFreeze();
	
	if (joint[joint_name].state_alt == FORWARD) {
		joint[joint_name].state_alt = BACKWARD;
		TriggerActiveStateAlt(joint_name, -1);
	} else if (joint[joint_name].state_alt == BACKWARD) {
		joint[joint_name].state_alt = HOLD;
		TriggerPassiveStateAlt(joint_name, joint[joint_name].strength_alt);
	} else if (joint[joint_name].state_alt == HOLD) {
		joint[joint_name].state_alt = RELAX;
		TriggerPassiveStateAlt(joint_name, 0);
	} else {
		joint[joint_name].state_alt = FORWARD;
		TriggerActiveStateAlt(joint_name, 1);
	}
}

void Restart()
{
	for (auto& [object_name, o] : object) {
		dBodySetPosition(
			o.dBody,
			o.position[0],
			o.position[1],
			o.position[2]);
		dBodySetQuaternion(o.dBody, o.orientation);
		dBodySetLinearVel(o.dBody, 0.00, 0.00, 0.00);
		dBodySetAngularVel(o.dBody, 0.00, 0.00, 0.00);

		o.freeze.position[0] = o.position[0];
		o.freeze.position[1] = o.position[1];
		o.freeze.position[2] = o.position[2];

		o.freeze.orientation[0] = o.orientation[0];
		o.freeze.orientation[1] = o.orientation[1];
		o.freeze.orientation[2] = o.orientation[2];
		o.freeze.orientation[3] = o.orientation[3];

		o.freeze.linearVel[0] = 0.00;
		o.freeze.linearVel[1] = 0.00;
		o.freeze.linearVel[2] = 0.00;

		o.freeze.angularVel[0] = 0.00;
		o.freeze.angularVel[1] = 0.00;
		o.freeze.angularVel[2] = 0.00;
	}
	
	for (auto& [body_name, b] : body) {
		dBodySetPosition(
			b.dBody,
			b.position[0],
			b.position[1],
			b.position[2]);
		dBodySetQuaternion(b.dBody, b.orientation);
		dBodySetLinearVel(b.dBody, 0.00, 0.00, 0.00);
		dBodySetAngularVel(b.dBody, 0.00, 0.00, 0.00);

		b.freeze.position[0] = b.position[0];
		b.freeze.position[1] = b.position[1];
		b.freeze.position[2] = b.position[2];

		b.freeze.orientation[0] = b.orientation[0];
		b.freeze.orientation[1] = b.orientation[1];
		b.freeze.orientation[2] = b.orientation[2];
		b.freeze.orientation[3] = b.orientation[3];

		b.freeze.linearVel[0] = 0.00;
		b.freeze.linearVel[1] = 0.00;
		b.freeze.linearVel[2] = 0.00;

		b.freeze.angularVel[0] = 0.00;
		b.freeze.angularVel[1] = 0.00;
		b.freeze.angularVel[2] = 0.00;
	}
	
	for (auto& [joint_name, j] : joint) {
		dBodySetPosition(
			j.dBody,
			j.position[0],
			j.position[1],
			j.position[2]);
		dBodySetQuaternion(j.dBody, j.orientation);
		dBodySetLinearVel(j.dBody, 0.00, 0.00, 0.00);
		dBodySetAngularVel(j.dBody, 0.00, 0.00, 0.00);

		j.freeze.position[0] = j.position[0];
		j.freeze.position[1] = j.position[1];
		j.freeze.position[2] = j.position[2];

		j.freeze.orientation[0] = j.orientation[0];
		j.freeze.orientation[1] = j.orientation[1];
		j.freeze.orientation[2] = j.orientation[2];
		j.freeze.orientation[3] = j.orientation[3];

		j.freeze.linearVel[0] = 0.00;
		j.freeze.linearVel[1] = 0.00;
		j.freeze.linearVel[2] = 0.00;

		j.freeze.angularVel[0] = 0.00;
		j.freeze.angularVel[1] = 0.00;
		j.freeze.angularVel[2] = 0.00;
	}
}

void CameraRotateZClockwise(Camera3D *camera, Vector3 *CameraZoom, Vector3 *CameraOffset)
{
	Matrix m = MatrixRotateZ(DEG2RAD*10);
	Quaternion q = QuaternionFromMatrix(m);
	Quaternion iq = QuaternionInvert(q);
	Quaternion p = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraOffset->x,
				CameraOffset->y,
				CameraOffset->z,
				0.0f
	});

	p = QuaternionMultiply(p, q);

	CameraOffset->x = p.x;
	CameraOffset->y = p.y;
	CameraOffset->z = p.z;

	Quaternion p2 = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraZoom->x,
				CameraZoom->y,
				CameraZoom->z,
				0.0f
	});

	p2 = QuaternionMultiply(p2, q);

	CameraZoom->x = p2.x;
	CameraZoom->y = p2.y;
	CameraZoom->y = p2.z;
}

void CameraRotateZCClockwise(Camera3D *camera, Vector3 *CameraZoom, Vector3 *CameraOffset)
{
	Matrix m = MatrixRotateZ(DEG2RAD*-10);
	Quaternion q = QuaternionFromMatrix(m);
	Quaternion iq = QuaternionInvert(q);
	Quaternion p = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraOffset->x,
				CameraOffset->y,
				CameraOffset->z,
				0.0f
	});

	p = QuaternionMultiply(p, q);

	CameraOffset->x = p.x;
	CameraOffset->y = p.y;
	CameraOffset->z = p.z;

	Quaternion p2 = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraZoom->x,
				CameraZoom->y,
				CameraZoom->z,
				0.0f
	});

	p2 = QuaternionMultiply(p2, q);

	CameraZoom->x = p2.x;
	CameraZoom->y = p2.y;
	CameraZoom->z = p2.z;
}

// TODO: FIX
void CameraRotateXClockwise(Camera3D *camera, Vector3 *CameraZoom, Vector3 *CameraOffset)
{
	Matrix m = MatrixRotateX(DEG2RAD*10);
	Quaternion q = QuaternionFromMatrix(m);
	Quaternion iq = QuaternionInvert(q);
	Quaternion p = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraOffset->x,
				CameraOffset->y,
				CameraOffset->z,
				0.0f
	});

	p = QuaternionMultiply(p, q);

	CameraOffset->x = p.x;
	CameraOffset->y = p.y;
	CameraOffset->z = p.z;

	Quaternion p2 = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraZoom->x,
				CameraZoom->y,
				CameraZoom->z,
				0.0f
	});

	p2 = QuaternionMultiply(p2, q);

	CameraZoom->x = p2.x;
	CameraZoom->y = p2.y;
	CameraZoom->y = p2.z;
}

// TODO: FIX
void CameraRotateXCClockwise(Camera3D *camera, Vector3 *CameraZoom, Vector3 *CameraOffset)
{
	Matrix m = MatrixRotateX(DEG2RAD*-10);
	Quaternion q = QuaternionFromMatrix(m);
	Quaternion iq = QuaternionInvert(q);
	Quaternion p = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraOffset->x,
				CameraOffset->y,
				CameraOffset->z,
				0.0f
	});

	p = QuaternionMultiply(p, q);

	CameraOffset->x = p.x;
	CameraOffset->y = p.y;
	CameraOffset->z = p.z;

	Quaternion p2 = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraZoom->x,
				CameraZoom->y,
				CameraZoom->z,
				0.0f
	});

	p2 = QuaternionMultiply(p2, q);

	CameraZoom->x = p2.x;
	CameraZoom->y = p2.y;
	CameraZoom->z = p2.z;
}

void CameraZoomIn(Camera3D *camera, Vector3 *CameraZoom, Vector3 *CameraOffset)
{
	CameraOffset->x = 0.9f * CameraOffset->x;
	CameraOffset->y = 0.9f * CameraOffset->y;
	CameraOffset->z = 0.9f * CameraOffset->z; 
}

void CameraZoomOut(Camera3D *camera, Vector3 *CameraZoom, Vector3 *CameraOffset)
{
	CameraOffset->x = 1.1f * CameraOffset->x;
	CameraOffset->y = 1.1f * CameraOffset->y;
	CameraOffset->z = 1.1f * CameraOffset->z;
}

void UpdatePlaycam(Camera3D *camera, Vector3 *CameraOffset)
{
	int size = body.size(); 
	if (size > 0) {
		float x = 0.00;
		float y = 0.00;
		float z = 0.00;
	
		for (auto const [body_name, b] : body) {
			if (game.freeze) {
				x = x + b.freeze.position[0];
				y = y + b.freeze.position[1];
				z = z + b.freeze.position[2];
			} else {
				const dReal *position = dBodyGetPosition(b.dBody);
				x = x + position[0];
				y = y + position[1];
				z = z + position[2];
			}
		}

		camera->target = (Vector3){ x/size, y/size, z/size };
		camera->position = (Vector3){
			CameraOffset->x + x/size,
			CameraOffset->y + y/size,
			CameraOffset->z + z/size,
		};
	}
}

void SelectJoint (Camera3D Camera, Ray MouseRay, RayCollision MouseCollision)
{
	MouseRay = GetMouseRay(GetMousePosition(), Camera);
	for (auto const& [joint_name, j] : joint) {
		MouseCollision = GetRayCollisionSphere(MouseRay,
			(Vector3){
				j.freeze.position[0],
				j.freeze.position[1],
				j.freeze.position[2],
			},
			j.radius
		);

		if (MouseCollision.hit) {
			game.selected_joint = j.name;
			break;
		} else {
			game.selected_joint = "NONE";
		}
	}
}

std::map<int, FrameData> RecordedFrames;

void RecordFrame(int gameframe)
{
	std::ofstream tempframefile("tempframefile.txt");
	tempframefile << "FRAME " << gameframe << "\n";
	for (auto const& [joint_name, j] : joint) {
		RecordedFrames[gameframe].joint[joint_name] = j;
		tempframefile << j.name << " " << j.state << " " << j.state_alt << "\n";
	}
	tempframefile.close();

	std::ofstream tempreplayfile("tempreplayfile.txt", std::ios::app);
	tempreplayfile << "FRAME " << gameframe << "\n";
	for (auto const& [joint_name, j] : joint) {
		RecordedFrames[gameframe].joint[joint_name] = j;
		tempreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
	}
	tempreplayfile.close();}

void PlayFrame(int gameframe)
{
	FrameData frame = RecordedFrames[gameframe];
	for (auto& [joint_name, j] : frame.joint) {
		switch(j.state) {
			case RELAX: {
				j.state = RELAX;
				TriggerPassiveState(j.name, 0);
			} break;
			case HOLD: {
				j.state = HOLD;
				TriggerPassiveState(j.name, j.strength);
			} break;
			case FORWARD: {
				j.state = FORWARD;
				TriggerActiveState(j.name, 1);
			} break; 
			case BACKWARD: {
				j.state = BACKWARD;
				TriggerActiveState(j.name, -1);
			} break;
		}
	
		switch(j.state_alt) {
			case RELAX: {
				j.state_alt = RELAX;
				TriggerPassiveStateAlt(j.name, 0);
			} break;
			case HOLD: {
				j.state_alt = HOLD;
				TriggerPassiveStateAlt(j.name, j.strength_alt);
			} break;
			case FORWARD: {
				j.state_alt = FORWARD;
				TriggerActiveStateAlt(j.name, 1);
			} break;
			case BACKWARD: {
				j.state_alt = BACKWARD;
				TriggerActiveStateAlt(j.name, -1);
			} break;
		}
	}
}

void EditReplay()
{
	game.gamemode = FREEPLAY;

	game.freeze = true;
	UpdateFreeze();

	std::ofstream tempframefile("tempframefile.txt");
	tempframefile << "FRAME " << game.gameframe << "\n";
	for (auto const& [joint_name, j] : joint) {
		tempframefile << j.name << " " << j.state << " " << j.state_alt << "\n";
	}
	tempframefile.close();

	std::ofstream tempreplayfile("tempreplayfile.txt");
	for (auto const& [gameframe, frame] : RecordedFrames) {
		if (gameframe > game.gameframe) {
			break;
		} else {
			tempreplayfile << "FRAME " << gameframe << "\n";
			for (auto const& [joint_name, j] : frame.joint) {
				tempreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
			}
		}
	}
	tempreplayfile.close();
}

void SaveReplay()
{
	std::ofstream savedreplayfile("savedreplayfile.txt");
	for (auto const& [gameframe, frame] : RecordedFrames) {
		savedreplayfile << "FRAME " << gameframe << "\n";
		for (auto const& [joint_name, j] : frame.joint) {
			savedreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
		}
	}
	savedreplayfile.close();
}

void StartFreeplay()
{
	game.gamemode = FREEPLAY;

	RelaxAll();
	RelaxAllAlt();

	game.freeze = true;
	game.gameframe = 0;
	game.freeze_t = 0;
	game.unfreeze_t = 0;
	
	Restart();

	UpdateFreeze();

	std::ofstream tempreplayfile("tempreplayfile.txt");
	tempreplayfile << "FRAME 0\n";
	for (auto const& [joint_name, j] : joint) {
		RecordedFrames[0].joint[joint_name] = j;
		tempreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
	}
	tempreplayfile.close();
}

void StartReplay()
{
	game.gamemode = REPLAY;

	RelaxAll();
	RelaxAllAlt();

	game.freeze = false;
	game.gameframe = 0;
	game.freeze_t = 0;
	game.unfreeze_t = 0;
	
	Restart();
}


void UpdateFrame()
{
	if (game.freeze) {
		if (!(++game.freeze_t < game.freeze_time)) {
			ReFreeze();
		}
	} else {
		if (game.gamemode == FREEPLAY) {
			if (!(++game.unfreeze_t < game.unfreeze_time)) {
				UpdateFreeze();
			}
		}
		++game.gameframe;
	}

	if (!game.pause) {
		if(!game.freeze && game.gamemode == FREEPLAY) {
			RecordFrame(game.gameframe);
		} else if (game.gamemode == REPLAY) {
			float size = RecordedFrames.size();
			if (game.gameframe < size) {
				PlayFrame(game.gameframe);
			} else if (game.gameframe > size + 100) {
				StartReplay();
			}
		}

		dSpaceCollide(game.space, 0, &nearCallback);
		dWorldStep(game.world, game.step);
		dJointGroupEmpty(game.contactgroup);
	}
}

int main()
{
	SetTraceLogLevel(LOG_ERROR);

	InitWindow(1600, 900, "TOBAS");

	Camera3D camera = { 0 };
	camera.up = (Vector3){ 0.00, 0.00, 1.00 };
	camera.fovy = 45.00;
	camera.projection = CAMERA_PERSPECTIVE;

	Vector3 CameraOffset = (Vector3){ 0.00, -5.00, 0.0f };
	Vector3 CameraZoom = (Vector3){ 0.00, 0.00, 0.00 };

	SetTargetFPS(60);
	
	GameStart();

	std::ofstream tempframefile("tempframefile.txt");
	tempframefile << "FRAME " << game.gameframe << "\n";
	for (auto const& [joint_name, j] : joint) {
		tempframefile << j.name << " " << j.state << " " << j.state_alt << "\n";
	}
	tempframefile.close();


	std::ofstream tempreplayfile("tempreplayfile.txt");
	tempreplayfile << "FRAME " << game.gameframe << "\n";
	for (auto const& [joint_name, j] : joint) {
		tempreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
	}
	tempreplayfile.close();

	Ray MouseRay = { 0 };
	RayCollision MouseCollision = { 0 };

	while (!WindowShouldClose()) {
		SetWindowTitle(TextFormat("TOBAS %dFPS", GetFPS()));

		UpdatePlaycam(&camera, &CameraOffset);

		SelectJoint(camera, MouseRay, MouseCollision);
		
		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			if (IsKeyDown(KEY_W))
				CameraRotateXClockwise(&camera, &CameraZoom, &CameraOffset);

			if (IsKeyDown(KEY_A))
				CameraRotateZClockwise(&camera, &CameraZoom, &CameraOffset);

			if (IsKeyDown(KEY_S))
				CameraRotateXCClockwise(&camera, &CameraZoom, &CameraOffset);

			if (IsKeyDown(KEY_D))
				CameraRotateZCClockwise(&camera, &CameraZoom, &CameraOffset);
		} else {
			if (IsKeyDown(KEY_W))
				CameraZoomIn(&camera, &CameraZoom, &CameraOffset);

			if (IsKeyDown(KEY_A))
				CameraRotateZClockwise(&camera, &CameraZoom, &CameraOffset);

			if (IsKeyDown(KEY_S))
				CameraZoomOut(&camera, &CameraZoom, &CameraOffset);

			if (IsKeyDown(KEY_D))
				CameraRotateZCClockwise(&camera, &CameraZoom, &CameraOffset);
		}

		if (IsKeyPressed(KEY_Z)) {
			if (IsKeyDown(KEY_LEFT_CONTROL)) {
				if (game.freeze && game.gamemode == FREEPLAY) {
					PlayFrame(game.gameframe - 1);
				}
			} else {
				if (IsKeyDown(KEY_LEFT_SHIFT)) {
					ToggleActiveStateAlt(game.selected_joint);
				} else {
					ToggleActiveState(game.selected_joint);
				}
			}
		}

		if (IsKeyPressed(KEY_X)) {
			if (IsKeyDown(KEY_LEFT_SHIFT)) {
				TogglePassiveStateAlt(game.selected_joint);
			} else {
				TogglePassiveState(game.selected_joint);
			}
		}

		if (IsKeyPressed(KEY_C)) {
			TogglePlayerPassiveStates();
			TogglePlayerPassiveStatesAlt();
		}

		if (IsMouseButtonPressed(0)) {
			if (IsKeyDown(KEY_LEFT_SHIFT)) {
				CycleStateAlt(game.selected_joint);
			} else {
				CycleState(game.selected_joint);
			}
		}

		if (IsKeyPressed(KEY_SPACE)) {
			if (game.freeze && game.gamemode == FREEPLAY) {
				if (IsKeyDown(KEY_LEFT_SHIFT)) {
					GameStep(1);
				} else {
					GameStep(game.turnframes);
				}
			} else if (!game.freeze && game.gamemode == REPLAY) {
				StartFreeplay();
			}
		}

		if (IsKeyPressed(KEY_E)) {
			if (!game.freeze && game.gamemode == REPLAY) {
				EditReplay();
			}
		}

		if (IsKeyPressed(KEY_F)) {
			SaveReplay();
		}

		if (IsKeyPressed(KEY_R)) {
			StartReplay();
		}

		if (IsKeyPressed(KEY_P)) {
			game.pause = game.pause == false;
		}

		UpdateFrame();
	
		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
				DrawFrame();
			EndMode3D();
			DrawText(TextFormat("%d", game.gameframe), 800, 20, 32, BLACK);
		EndDrawing();
	}
	
	GameEnd();
	CloseWindow();

	return 0;
}
