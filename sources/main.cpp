#include <iostream>
#include <string>
#include <map>

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

#include <ode/ode.h>
#include <lua.hpp>

enum GameContext
{
	NoContext,
	ObjectContext,
	BodyContext,
	JointContext
} DataContext = NoContext;

enum PhysicsShape
{
	Box,
	Sphere,
	Capsule,
	Cylinder
};

enum PhysicsJoint
{
	Hinge,
	Hinge2,
	Slider,
	Universal
};

enum JointState
{
	HOLD,
	RELAX,
	FORWARD,
	BACKWARD,
	ALT_HOLD,
	ALT_RELAX,
	ALT_FORWARD,
	ALT_BACKWARD
};

struct FrameData
{
	const char* joint_name;
	JointState joint_state;
};

struct plane
{
	dGeomID dGeom;
	bool state;
};

struct game
{
	dWorldID world;
	dSpaceID space;
	dJointGroupID contactgroup;
	plane globalplane;
	dReal gravity[3];
	dReal freeze_time;
	dReal freeze_t;
	dReal unfreeze_time;
	dReal unfreeze_t;
	dReal step;
	int game_frame;
	int turn_frame;
	bool isFrozen;
} game;

struct FreezeData
{
	dReal position[3];
	dReal orientation[4];
	dReal angularVel[3];
	dReal linearVel[3];
};

struct Composite
{
	std::string name;
	dBodyID dBody;
};

struct ObjectData
{
	std::string name;
	enum PhysicsShape shape;
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
	bool isStatic;
	bool isComposite;
};

struct BodyData
{
	std::string name;
	enum PhysicsShape shape;
	dReal radius;
	dReal length;
	dReal density;
	dReal sides[3];
	dReal position[3];
	dReal orientation[4];
	dBodyID dBody;
	dGeomID dGeom;
	Composite composite;
	FreezeData freeze;
	bool isStatic;
	bool isComposite;
};

struct JointData
{
	std::string name;
	enum PhysicsShape shape;
	dReal radius;
	dReal length;
	dReal density;
	dReal sides[3];
	dReal position[3];
	dReal orientation[4];
	dBodyID dBody;
	dGeomID dGeom;
	dJointID dJoint[2];
	bool isStatic;
	bool isComposite;
	dReal axis[3];
	dReal altAxis[3];
	dReal range[2];
	dReal altRange[2];
	dReal strength;
	dReal altStrength;
	dReal velocity;
	dReal altVelocity;
	std::string connections[2];
	enum PhysicsJoint connectionType;
	Composite composite;
	FreezeData freeze;
	JointState state;
	bool passiveState;
	bool activeState;
	bool altPassiveState;
	bool altActiveState;
};

bool PhysicsPaused = false;

bool GlobalPassiveState = false;

int MAX_CONTACTS = 4;

std::string MSG;

std::string SelectedJoint = "NONE";

Color DynamicObjectColor = (Color){ 255, 255, 255, 255 };
Color StaticObjectColor = (Color){ 51, 51, 51, 255 };
Color JointColor = (Color){ 155, 155, 155, 255 };
Color BodyColor = (Color){ 255, 255, 255, 255 };
Color GhostColor = (Color){ 51, 51, 51, 51 };

std::map<std::string, ObjectData> Object;
std::map<std::string, BodyData> Body;
std::map<std::string, JointData> Joint;

std::string ObjectKey;
std::string BodyKey;
std::string JointKey;

static void nearCallback (void *, dGeomID o1, dGeomID o2)
{
	//TODO: Fix Collisions
	if (o1 != game.globalplane.dGeom && o2 != game.globalplane.dGeom) return;
 
	int i;

	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	dContact contact[MAX_CONTACTS];

	for (i = 0; i < MAX_CONTACTS; i++) {
		contact[i].surface.mode = dContactApprox1;
		contact[i].surface.mu = 5000.0f;
	}

	if (int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact))) {
		for (i = 0; i < numc; i++) {
			dJointID c = dJointCreateContact(game.world, game.contactgroup, contact + i);
			dJointAttach(c, b1, b2);
		}
	}
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

int globalplane(lua_State* L)
{
	switch(DataContext)
	{
		case NoContext: {
			game.globalplane.state = true;
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

int object(lua_State* L)
{
	DataContext = ObjectContext;

	std::string name = lua_tostring(L, -1);
	
	ObjectKey = name;

	Object[ObjectKey].name = name;

	Object[ObjectKey].orientation[0] = 1.00;
	Object[ObjectKey].orientation[1] = 0.00;
	Object[ObjectKey].orientation[2] = 0.00;
	Object[ObjectKey].orientation[3] = 0.00;

	Object[ObjectKey].freeze.orientation[0] = 1.00;
	Object[ObjectKey].freeze.orientation[1] = 0.00;
	Object[ObjectKey].freeze.orientation[2] = 0.00;
	Object[ObjectKey].freeze.orientation[3] = 0.00;

	Object[ObjectKey].freeze.linearVel[0] = 0.00;
	Object[ObjectKey].freeze.linearVel[1] = 0.00;
	Object[ObjectKey].freeze.linearVel[2] = 0.00;

	Object[ObjectKey].freeze.angularVel[0] = 0.00;
	Object[ObjectKey].freeze.angularVel[1] = 0.00;
	Object[ObjectKey].freeze.angularVel[2] = 0.00;

	Object[ObjectKey].isStatic = false;
	Object[ObjectKey].isComposite = false;

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int body(lua_State* L)
{
	DataContext = BodyContext;

	std::string name = lua_tostring(L, -1);
	
	BodyKey = name;

	Body[BodyKey].name = name;

	Body[BodyKey].orientation[0] = 1.00;
	Body[BodyKey].orientation[1] = 0.00;
	Body[BodyKey].orientation[2] = 0.00;
	Body[BodyKey].orientation[3] = 0.00;

	Body[BodyKey].freeze.orientation[0] = 1.00;
	Body[BodyKey].freeze.orientation[1] = 0.00;
	Body[BodyKey].freeze.orientation[2] = 0.00;
	Body[BodyKey].freeze.orientation[3] = 0.00;

	Body[BodyKey].freeze.linearVel[0] = 0.00;
	Body[BodyKey].freeze.linearVel[1] = 0.00;
	Body[BodyKey].freeze.linearVel[2] = 0.00;

	Body[BodyKey].freeze.angularVel[0] = 0.00;
	Body[BodyKey].freeze.angularVel[1] = 0.00;
	Body[BodyKey].freeze.angularVel[2] = 0.00;

	Body[BodyKey].isStatic = false;
	Body[BodyKey].isComposite = false;

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int joint(lua_State* L)
{
	DataContext = JointContext;

	std::string name = lua_tostring(L, -1);

	JointKey = name;

	Joint[JointKey].name = name;

	Joint[JointKey].orientation[0] = 1.00;
	Joint[JointKey].orientation[1] = 0.00;
	Joint[JointKey].orientation[2] = 0.00;
	Joint[JointKey].orientation[3] = 0.00;

	Joint[JointKey].freeze.orientation[0] = 1.00;
	Joint[JointKey].freeze.orientation[1] = 0.00;
	Joint[JointKey].freeze.orientation[2] = 0.00;
	Joint[JointKey].freeze.orientation[3] = 0.00;

	Joint[JointKey].freeze.linearVel[0] = 0.00;
	Joint[JointKey].freeze.linearVel[1] = 0.00;
	Joint[JointKey].freeze.linearVel[2] = 0.00;

	Joint[JointKey].freeze.angularVel[0] = 0.00;
	Joint[JointKey].freeze.angularVel[1] = 0.00;
	Joint[JointKey].freeze.angularVel[2] = 0.00;

	Joint[JointKey].state = RELAX;
	Joint[JointKey].passiveState = false;
	Joint[JointKey].activeState = false;
	Joint[JointKey].altPassiveState = false;
	Joint[JointKey].altActiveState = false;

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
		case ObjectContext: {
			if ("box" == shape) {
				Object[ObjectKey].shape = Box;
			} else if ("sphere" == shape) {
					Object[ObjectKey].shape = Sphere;
			} else if ("capsule" == shape) {
					Object[ObjectKey].shape = Capsule;
			} else if ("cylinder" == shape) {
					Object[ObjectKey].shape = Cylinder;
			} else  {
				//Error Handling
			}
		} break;
		case BodyContext: {
			if ("box" == shape) {
				Body[BodyKey].shape = Box;
			} else if ("sphere" == shape) {
				Body[BodyKey].shape = Sphere;
			} else if ("capsule" == shape) {
				Body[BodyKey].shape = Capsule;
			} else if ("cylinder" == shape) {
				Body[BodyKey].shape = Cylinder;
			} else  {
				//Error Handling
			}
		} break;
		case JointContext: {
			if ("box"  == shape) {
				Joint[JointKey].shape = Box;
			} else if ("sphere" == shape) {
				Joint[JointKey].shape = Sphere;
			} else if ("capsule" == shape) {
				Joint[JointKey].shape = Capsule;
			} else if ("cylinder" == shape) {
				Joint[JointKey].shape = Cylinder;
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
		case ObjectContext: {
			Object[ObjectKey].position[0] = position[0];
			Object[ObjectKey].position[1] = position[1];
			Object[ObjectKey].position[2] = position[2];
	
			Object[ObjectKey].freeze.position[0] = position[0];
			Object[ObjectKey].freeze.position[1] = position[1];
			Object[ObjectKey].freeze.position[2] = position[2];
		} break;
		case BodyContext: {
			Body[BodyKey].position[0] = position[0];
			Body[BodyKey].position[1] = position[1];
			Body[BodyKey].position[2] = position[2];
	
			Body[BodyKey].freeze.position[0] = position[0];
			Body[BodyKey].freeze.position[1] = position[1];
			Body[BodyKey].freeze.position[2] = position[2];
		} break;
		case JointContext: {
			Joint[JointKey].position[0] = position[0];
			Joint[JointKey].position[1] = position[1];
			Joint[JointKey].position[2] = position[2];
	
			Joint[JointKey].freeze.position[0] = position[0];
			Joint[JointKey].freeze.position[1] = position[1];
			Joint[JointKey].freeze.position[2] = position[2];
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
		case ObjectContext: {
			Object[ObjectKey].orientation[0] = orientation[0];
			Object[ObjectKey].orientation[1] = orientation[1];
			Object[ObjectKey].orientation[2] = orientation[2];
			Object[ObjectKey].orientation[3] = orientation[3];

			Object[ObjectKey].freeze.orientation[0] = orientation[0];
			Object[ObjectKey].freeze.orientation[1] = orientation[1];
			Object[ObjectKey].freeze.orientation[2] = orientation[2];
			Object[ObjectKey].freeze.orientation[3] = orientation[3];
		} break;
		case BodyContext: {
			Body[BodyKey].orientation[0] = orientation[0];
			Body[BodyKey].orientation[1] = orientation[1];
			Body[BodyKey].orientation[2] = orientation[2];
			Body[BodyKey].orientation[3] = orientation[3];

			Body[BodyKey].freeze.orientation[0] = orientation[0];
			Body[BodyKey].freeze.orientation[1] = orientation[1];
			Body[BodyKey].freeze.orientation[2] = orientation[2];
			Body[BodyKey].freeze.orientation[3] = orientation[3];
		} break;
		case JointContext: {
			Joint[JointKey].orientation[0] = orientation[0];
			Joint[JointKey].orientation[1] = orientation[1];
			Joint[JointKey].orientation[2] = orientation[2];
			Joint[JointKey].orientation[3] = orientation[3];
	
			Joint[JointKey].freeze.orientation[0] = orientation[0];
			Joint[JointKey].freeze.orientation[1] = orientation[1];
			Joint[JointKey].freeze.orientation[2] = orientation[2];
			Joint[JointKey].freeze.orientation[3] = orientation[3];
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
		case ObjectContext: {
			Object[ObjectKey].sides[0] = sides[0];
			Object[ObjectKey].sides[1] = sides[1];
			Object[ObjectKey].sides[2] = sides[2];
		} break;
		case BodyContext: {
			Body[BodyKey].sides[0] = sides[0];
			Body[BodyKey].sides[1] = sides[1];
			Body[BodyKey].sides[2] = sides[2];
		} break;
		case JointContext: {
			Joint[JointKey].sides[0] = sides[0];
			Joint[JointKey].sides[1] = sides[1];
			Joint[JointKey].sides[2] = sides[2];
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
		case ObjectContext: {
			Object[ObjectKey].density = density;
		} break;
		case BodyContext: {
			Body[BodyKey].density = density;
		} break;
		case JointContext: {
			Joint[JointKey].density = density;
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int static_object(lua_State* L)
{
	switch(DataContext)
	{
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			Object[ObjectKey].isStatic = true;
		} break;
		case BodyContext: {
			Body[BodyKey].isStatic = true;
		} break;
		case JointContext: {
			Joint[JointKey].isStatic = true;
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
		case ObjectContext: {
			Object[ObjectKey].radius = radius;
		} break;
		case BodyContext: {
			Body[BodyKey].radius = radius;
		} break;
		case JointContext: {
			Joint[JointKey].radius = radius;
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
		case ObjectContext: {
			Object[ObjectKey].length = length;
		} break;
		case BodyContext: {
			Body[BodyKey].length = length;
		} break;
		case JointContext: {
			Joint[JointKey].length = length;
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
		case ObjectContext: {
			Object[ObjectKey].isComposite = true;
			Object[ObjectKey].composite.name = name;
		} break;
		case BodyContext: {
			Body[BodyKey].isComposite = true;
			Body[BodyKey].composite.name = name;
		} break;
		case JointContext: {
			Joint[JointKey].isComposite = true;
			Joint[JointKey].composite.name = name;
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
			Joint[JointKey].strength = strength;
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int altStrength(lua_State* L)
{
	lua_Number altStrength;
		
	lua_rawgeti(L, -1, 1);
	altStrength = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case JointContext: {
			Joint[JointKey].altStrength = altStrength;
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
			Joint[JointKey].velocity = velocity;
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int altVelocity(lua_State* L)
{
	lua_Number altVelocity;
		
	lua_rawgeti(L, -1, 1);
	altVelocity = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case JointContext: {
			Joint[JointKey].altVelocity = altVelocity;
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
			Joint[JointKey].axis[0] = axis[0];
			Joint[JointKey].axis[1] = axis[1];
			Joint[JointKey].axis[2] = axis[2];
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int altAxis(lua_State* L)
{
	lua_Number altAxis[3];
		
	lua_rawgeti(L, -1, 1);
	altAxis[0] = lua_tonumber(L, -1); 

	lua_rawgeti(L, -2, 2);
	altAxis[1] = lua_tonumber(L, -1); 

	lua_rawgeti(L, -3, 3);
	altAxis[2] = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case JointContext: {
			Joint[JointKey].altAxis[0] = altAxis[0];
			Joint[JointKey].altAxis[1] = altAxis[1];
			Joint[JointKey].altAxis[2] = altAxis[2];
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
			Joint[JointKey].range[0] = range[0];
			Joint[JointKey].range[1] = range[1];
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int altRange(lua_State* L)
{
	lua_Number altRange[2];
		
	lua_rawgeti(L, -1, 1);
	altRange[0] = lua_tonumber(L, -1); 

	lua_rawgeti(L, -2, 2);
	altRange[1] = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case JointContext: {
			Joint[JointKey].altRange[0] = altRange[0];
			Joint[JointKey].altRange[1] = altRange[1];
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
			Joint[JointKey].connections[0] = connections[0];
			Joint[JointKey].connections[1] = connections[1];
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
				Joint[JointKey].connectionType = Hinge;
			} else if ("slider" == connectionType) {
				Joint[JointKey].connectionType = Slider;
			} else if ("universal" == connectionType) {
				Joint[JointKey].connectionType = Universal;
			} else if ("hinge2" == connectionType) {
				Joint[JointKey].connectionType = Hinge2;
			} else {
			// Error Handling
			}
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

void GameInit()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushcfunction(L, gravity);
	lua_setglobal(L, "gravity");

	lua_pushcfunction(L, globalplane);
	lua_setglobal(L, "globalplane");

	lua_pushcfunction(L, object);
	lua_setglobal(L, "object");

	lua_pushcfunction(L, body);
	lua_setglobal(L, "body");

	lua_pushcfunction(L, joint);
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

	lua_pushcfunction(L, static_object);
	lua_setglobal(L, "static");

	lua_pushcfunction(L, radius);
	lua_setglobal(L, "radius");

	lua_pushcfunction(L, length);
	lua_setglobal(L, "length");

	lua_pushcfunction(L, composite);
	lua_setglobal(L, "composite");

	lua_pushcfunction(L, strength);
	lua_setglobal(L, "strength");

	lua_pushcfunction(L, altStrength);
	lua_setglobal(L, "altStrength");

	lua_pushcfunction(L, velocity);
	lua_setglobal(L, "velocity");

	lua_pushcfunction(L, altVelocity);
	lua_setglobal(L, "altVelocity");

	lua_pushcfunction(L, axis);
	lua_setglobal(L, "axis");

	lua_pushcfunction(L, altAxis);
	lua_setglobal(L, "altAxis");

	lua_pushcfunction(L, axis);
	lua_setglobal(L, "axis");

	lua_pushcfunction(L, altAxis);
	lua_setglobal(L, "altAxis");

	lua_pushcfunction(L, range);
	lua_setglobal(L, "range");

	lua_pushcfunction(L, altRange);
	lua_setglobal(L, "altRange");

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
	GameInit();

	dMass mass;

	dInitODE2(0);

	game.step = 0.01f;
	game.game_frame = 0;
	game.turn_frame = 10;
	game.isFrozen = true;
	game.freeze_time = 40;
	game.freeze_t = 0;
	game.unfreeze_time = 1;
	game.unfreeze_t = 0;

	game.world = dWorldCreate();
	game.space = dHashSpaceCreate(0);
  	game.contactgroup = dJointGroupCreate(0);
	if (game.globalplane.state == true)
		game.globalplane.dGeom = dCreatePlane(game.space, 0, 0, 1, 0);

  	dWorldSetGravity(game.world, game.gravity[0], game.gravity[1], game.gravity[2]);
	
	std::map<std::string, ObjectData>::iterator o = Object.begin();

	while (o != Object.end()) {
		o->second.dBody = dBodyCreate(game.world);
		dBodySetPosition(
			o->second.dBody,
			o->second.position[0],
			o->second.position[1],
			o->second.position[2]
		);

		dBodySetQuaternion(o->second.dBody, o->second.orientation);
		
		switch(o->second.shape)
		{
			case Box: {
				o->second.dGeom = dCreateBox(game.space, o->second.sides[0], o->second.sides[1], o->second.sides[2]);
				dMassSetBox(&mass, o->second.density, o->second.sides[0], o->second.sides[1], o->second.sides[2]);
			} break;
			case Sphere: {
				o->second.dGeom = dCreateSphere(game.space, o->second.radius);
				dMassSetSphere(&mass, o->second.density, o->second.radius);
			} break;
			case Capsule: {
				o->second.dGeom = dCreateCapsule(game.space, o->second.radius, o->second.length);
				dMassSetCapsule(&mass, o->second.density, 1, o->second.length, o->second.radius);
			} break;
			case Cylinder: {
				o->second.dGeom = dCreateCylinder(game.space, o->second.radius, o->second.length);
				dMassSetCylinder(&mass, o->second.density, 1, o->second.length, o->second.radius);
			} break;
		}

		dMassAdjust(&mass, 0.5);
		dBodySetMass(o->second.dBody, &mass);
		dGeomSetBody(o->second.dGeom, o->second.dBody);

		if (o->second.isStatic) {
			// I don't like how the engine makes statiic objects
			dJointID fixed = dJointCreateFixed(game.world ,0);
			dJointAttach(fixed, o->second.dBody, 0);
			dJointSetFixed(fixed);
		}

		++o;
	}

	std::map<std::string, BodyData>::iterator b = Body.begin();

	while (b != Body.end()) {
		b->second.dBody = dBodyCreate(game.world);
		dBodySetPosition(
			b->second.dBody,
			b->second.position[0],
			b->second.position[1],
			b->second.position[2]
		);

		dBodySetQuaternion(b->second.dBody, b->second.orientation);

		switch(b->second.shape)
		{
			case Box: {
				b->second.dGeom = dCreateBox(game.space, b->second.sides[0], b->second.sides[1], b->second.sides[2]);
				dMassSetBox(&mass, b->second.density, b->second.sides[0], b->second.sides[1], b->second.sides[2]);
			} break;
			case Sphere: {
				b->second.dGeom = dCreateSphere(game.space, b->second.radius);
				dMassSetSphere(&mass, b->second.density, b->second.radius);
			} break;
			case Capsule: {
				b->second.dGeom = dCreateCapsule(game.space, b->second.radius, b->second.length);
				dMassSetCapsule(&mass, b->second.density, 1, b->second.radius, b->second.length);
			} break;
			case Cylinder: {
				b->second.dGeom = dCreateCylinder(game.space, b->second.radius, b->second.length);
				dMassSetCylinder(&mass, b->second.density, 1, b->second.radius, b->second.length);
			} break;
		}

		dMassAdjust(&mass, 0.5);
		dBodySetMass(b->second.dBody, &mass);
		dGeomSetBody(b->second.dGeom, b->second.dBody);

		if (b->second.isStatic) {
			// I don't like how the engine makes statiic objects
			dJointID fixed = dJointCreateFixed(game.world ,0);
			dJointAttach(fixed, b->second.dBody, 0);
			dJointSetFixed(fixed);
		}

		++b;
	}

	std::map<std::string, JointData>::iterator j = Joint.begin();

	while (j != Joint.end()) {
		j->second.dBody = dBodyCreate(game.world);
		dBodySetPosition(
			j->second.dBody,
			j->second.position[0],
			j->second.position[1],
			j->second.position[2]
		);

		dBodySetQuaternion(j->second.dBody, j->second.orientation);

		switch(j->second.shape)
		{
			case Box: {
				j->second.dGeom = dCreateBox(game.space, j->second.sides[0], j->second.sides[1], j->second.sides[2]);
				dMassSetBox(&mass, j->second.density, j->second.sides[0], j->second.sides[1], j->second.sides[2]);
			} break;
			case Sphere: {
				j->second.dGeom = dCreateSphere(game.space, j->second.radius);
				dMassSetSphere(&mass, j->second.density, j->second.radius);
			} break;
			case Capsule: {
				j->second.dGeom = dCreateCapsule(game.space, j->second.radius, j->second.length);
				dMassSetCapsule(&mass, j->second.density, 1, j->second.radius, j->second.length);
			} break;
			case Cylinder: {
				j->second.dGeom = dCreateCylinder(game.space, j->second.radius, j->second.length);
				dMassSetCylinder(&mass, j->second.density, 1, j->second.radius, j->second.length);
			} break;
		}

		dMassAdjust(&mass, 0.5);
		dBodySetMass(j->second.dBody, &mass);
		dGeomSetBody(j->second.dGeom, j->second.dBody);

		if (j->second.isStatic) {
			// I don't like how the engine makes statiic objects
			dJointID fixed = dJointCreateFixed(game.world ,0);
			dJointAttach(fixed, j->second.dBody, 0);
			dJointSetFixed(fixed);
		}
		switch(j->second.connectionType)
		{
			case Hinge: {
				j->second.dJoint[0] = dJointCreateHinge(game.world, 0);
				dJointAttach(
					j->second.dJoint[0],
					Body[j->second.connections[0]].dBody,
					j->second.dBody);
				dJointSetHingeAnchor(
					j->second.dJoint[0],
					j->second.position[0],
					j->second.position[1],
					j->second.position[2]);
				dJointSetHingeAxis(
					j->second.dJoint[0],
					j->second.axis[0],
					j->second.axis[1],
					j->second.axis[2]);
		
				dJointSetHingeParam(
					j->second.dJoint[0],
					dParamLoStop,
					j->second.range[0]);
				dJointSetHingeParam(
					j->second.dJoint[0],
					dParamHiStop,
					j->second.range[1]);
		
				j->second.dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(
					j->second.dJoint[1],
					j->second.dBody,
					Body[j->second.connections[1]].dBody);
				dJointSetFixed(j->second.dJoint[1]);
			} break;
			case Slider: {
				j->second.dJoint[0] = dJointCreateSlider(game.world, 0);
				dJointAttach(
					j->second.dJoint[0],
					Body[j->second.connections[0]].dBody,
					j->second.dBody);
				dJointSetSliderAxis(
					j->second.dJoint[0],
					j->second.axis[0],
					j->second.axis[1],
					j->second.axis[2]);
		
				dJointSetSliderParam(
					j->second.dJoint[0],
					dParamLoStop,
					j->second.range[0]);
				dJointSetSliderParam(
					j->second.dJoint[0],
					dParamHiStop,
					j->second.range[1]);
		
				j->second.dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(
					j->second.dJoint[1],
					j->second.dBody,
					Body[j->second.connections[1]].dBody);
				dJointSetFixed(j->second.dJoint[1]);
			} break;
			case Universal: {
				j->second.dJoint[0] = dJointCreateUniversal(game.world, 0);
				dJointAttach(
					j->second.dJoint[0],
					Body[j->second.connections[0]].dBody,
					j->second.dBody);
				dJointSetUniversalAnchor(
					j->second.dJoint[0],
					j->second.position[0],
					j->second.position[1],
					j->second.position[2]);

				dJointSetUniversalAnchor(
					j->second.dJoint[0],
					j->second.position[0],
					j->second.position[1],
					j->second.position[2]);

				dJointSetUniversalAxis1(
					j->second.dJoint[0],
					j->second.axis[0],
					j->second.axis[1],
					j->second.axis[2]);
				dJointSetUniversalAxis2(
					j->second.dJoint[0],
					j->second.altAxis[0],
					j->second.altAxis[1],
					j->second.altAxis[2]);
		
				dJointSetUniversalParam(
					j->second.dJoint[0],
					dParamLoStop,
					j->second.range[0]);

				dJointSetUniversalParam(
					j->second.dJoint[0],
					dParamLoStop2,
					j->second.altRange[0]);

				dJointSetUniversalParam(
					j->second.dJoint[0],
					dParamHiStop,
					j->second.range[1]);

				dJointSetUniversalParam(
					j->second.dJoint[0],
					dParamHiStop2,
					j->second.altRange[1]);
		
				j->second.dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(
					j->second.dJoint[1],
					j->second.dBody,
					Body[j->second.connections[1]].dBody);
				dJointSetFixed(j->second.dJoint[1]);
			} break;
			case Hinge2: {
				j->second.dJoint[0] = dJointCreateHinge2(game.world, 0);
				dJointAttach(
					j->second.dJoint[0],
					Body[j->second.connections[0]].dBody,
					j->second.dBody);
				dJointSetHinge2Anchor(
					j->second.dJoint[0],
					j->second.position[0],
					j->second.position[1],
					j->second.position[2]);

				dJointSetHinge2Anchor(
					j->second.dJoint[0],
					j->second.position[0],
					j->second.position[1],
					j->second.position[2]);

				dJointSetHinge2Axis1(
					j->second.dJoint[0],
					j->second.axis[0],
					j->second.axis[1],
					j->second.axis[2]);
				dJointSetHinge2Axis2(
					j->second.dJoint[0],
					j->second.altAxis[0],
					j->second.altAxis[1],
					j->second.altAxis[2]);
		
				dJointSetHinge2Param(
					j->second.dJoint[0],
					dParamLoStop,
					j->second.range[0]);

				dJointSetHinge2Param(
					j->second.dJoint[0],
					dParamLoStop2,
					j->second.altRange[0]);

				dJointSetHinge2Param(
					j->second.dJoint[0],
					dParamHiStop,
					j->second.range[1]);

				dJointSetHinge2Param(
					j->second.dJoint[0],
					dParamHiStop2,
					j->second.altRange[1]);
		
				j->second.dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(
					j->second.dJoint[1],
					j->second.dBody,
					Body[j->second.connections[1]].dBody);
				dJointSetFixed(j->second.dJoint[1]);
			} break;
			default:
				j->second.dJoint[0] = dJointCreateFixed(game.world, 0);
				dJointAttach(
					j->second.dJoint[0],
					Body[j->second.connections[0]].dBody,
					j->second.dBody);
				dJointSetFixed(j->second.dJoint[0]);

				j->second.dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(
					j->second.dJoint[1],
					j->second.dBody,
					Body[j->second.connections[1]].dBody);
				dJointSetFixed(j->second.dJoint[1]);
		}

		++j;
	}
}

void UpdateFreeze()
{
	game.isFrozen = true;
	game.unfreeze_t = 0;

	std::map<std::string, ObjectData>::iterator o = Object.begin();

	while (o != Object.end()) {
		const dReal *position = dBodyGetPosition(o->second.dBody);
		const dReal *orientation = dBodyGetQuaternion(o->second.dBody);
		const dReal *linearVel = dBodyGetLinearVel(o->second.dBody);
		const dReal *angularVel = dBodyGetAngularVel(o->second.dBody);

		o->second.freeze.position[0] = position[0];
		o->second.freeze.position[1] = position[1];
		o->second.freeze.position[2] = position[2];

		o->second.freeze.orientation[0] = orientation[0];
		o->second.freeze.orientation[1] = orientation[1];
		o->second.freeze.orientation[2] = orientation[2];
		o->second.freeze.orientation[3] = orientation[3];

		o->second.freeze.linearVel[0] = linearVel[0];
		o->second.freeze.linearVel[1] = linearVel[1];
		o->second.freeze.linearVel[2] = linearVel[2];

		o->second.freeze.angularVel[0] = angularVel[0];
		o->second.freeze.angularVel[1] = angularVel[1];
		o->second.freeze.angularVel[2] = angularVel[2];

		++o;
	}

	std::map<std::string, BodyData>::iterator b = Body.begin();

	while (b != Body.end()) {
		const dReal *position = dBodyGetPosition(b->second.dBody);
		const dReal *orientation = dBodyGetQuaternion(b->second.dBody);
		const dReal *linearVel = dBodyGetLinearVel(b->second.dBody);
		const dReal *angularVel = dBodyGetAngularVel(b->second.dBody);

		b->second.freeze.position[0] = position[0];
		b->second.freeze.position[1] = position[1];
		b->second.freeze.position[2] = position[2];

		b->second.freeze.orientation[0] = orientation[0];
		b->second.freeze.orientation[1] = orientation[1];
		b->second.freeze.orientation[2] = orientation[2];
		b->second.freeze.orientation[3] = orientation[3];

		b->second.freeze.linearVel[0] = linearVel[0];
		b->second.freeze.linearVel[1] = linearVel[1];
		b->second.freeze.linearVel[2] = linearVel[2];

		b->second.freeze.angularVel[0] = angularVel[0];
		b->second.freeze.angularVel[1] = angularVel[1];
		b->second.freeze.angularVel[2] = angularVel[2];

		++b;
	}

	std::map<std::string, JointData>::iterator j = Joint.begin();

	while (j != Joint.end()) {
		const dReal *position = dBodyGetPosition(j->second.dBody);
		const dReal *orientation = dBodyGetQuaternion(j->second.dBody);
		const dReal *linearVel = dBodyGetLinearVel(j->second.dBody);
		const dReal *angularVel = dBodyGetAngularVel(j->second.dBody);

		j->second.freeze.position[0] = position[0];
		j->second.freeze.position[1] = position[1];
		j->second.freeze.position[2] = position[2];

		j->second.freeze.orientation[0] = orientation[0];
		j->second.freeze.orientation[1] = orientation[1];
		j->second.freeze.orientation[2] = orientation[2];
		j->second.freeze.orientation[3] = orientation[3];

		j->second.freeze.linearVel[0] = linearVel[0];
		j->second.freeze.linearVel[1] = linearVel[1];
		j->second.freeze.linearVel[2] = linearVel[2];

		j->second.freeze.angularVel[0] = angularVel[0];
		j->second.freeze.angularVel[1] = angularVel[1];
		j->second.freeze.angularVel[2] = angularVel[2];

		++j;
	}
}

void RelaxAll()
{
	GlobalPassiveState = false;

	dReal s = 0.0f;

	std::map<std::string, JointData>::iterator j = Joint.begin();

	while (j != Joint.end()) {
		switch(j->second.connectionType) {
			case Hinge: {
				dJointSetHingeParam(j->second.dJoint[0], dParamFMax, s);
				dJointSetHingeParam(j->second.dJoint[0], dParamVel, 0.0f);
			} break;
			case Slider: {
				dJointSetSliderParam(j->second.dJoint[0], dParamFMax, s);
				dJointSetSliderParam(j->second.dJoint[0], dParamVel, 0.0f);
			} break;
			case Universal: {
				dJointSetUniversalParam(j->second.dJoint[0], dParamFMax, s);
				dJointSetUniversalParam(j->second.dJoint[0], dParamVel, 0.0f);
				dJointSetUniversalParam(j->second.dJoint[0], dParamFMax2, s);
				dJointSetUniversalParam(j->second.dJoint[0], dParamVel2, 0.0f);
			} break;
			case Hinge2: {
				dJointSetHinge2Param(j->second.dJoint[0], dParamFMax, s);
				dJointSetHinge2Param(j->second.dJoint[0], dParamVel, 0.0f);
				dJointSetHinge2Param(j->second.dJoint[0], dParamFMax2, s);
				dJointSetHinge2Param(j->second.dJoint[0], dParamVel2, 0.0f);
			} break;
		}
		++j;
	}
}

void GameReset()
{	
	RelaxAll();

	game.isFrozen = true;
	game.game_frame = 0;
	game.freeze_t = 0;
	game.unfreeze_t = 0;

	std::map<std::string, ObjectData>::iterator o = Object.begin();

	while (o != Object.end()) {
		dBodySetPosition(
			o->second.dBody,
			o->second.position[0],
			o->second.position[1],
			o->second.position[2]);
		dBodySetQuaternion(o->second.dBody, o->second.orientation);
		dBodySetLinearVel(o->second.dBody, 0.00, 0.00, 0.00);
		dBodySetAngularVel(o->second.dBody, 0.00, 0.00, 0.00);

		o->second.freeze.position[0] = o->second.position[0];
		o->second.freeze.position[1] = o->second.position[1];
		o->second.freeze.position[2] = o->second.position[2];

		o->second.freeze.orientation[0] = o->second.orientation[0];
		o->second.freeze.orientation[1] = o->second.orientation[1];
		o->second.freeze.orientation[2] = o->second.orientation[2];
		o->second.freeze.orientation[3] = o->second.orientation[3];

		o->second.freeze.linearVel[0] = 0.00;
		o->second.freeze.linearVel[1] = 0.00;
		o->second.freeze.linearVel[2] = 0.00;

		o->second.freeze.angularVel[0] = 0.00;
		o->second.freeze.angularVel[1] = 0.00;
		o->second.freeze.angularVel[2] = 0.00;

		++o;
	}

	std::map<std::string, BodyData>::iterator b = Body.begin();

	while (b != Body.end()) {
		dBodySetPosition(
			b->second.dBody,
			b->second.position[0],
			b->second.position[1],
			b->second.position[2]);
		dBodySetQuaternion(b->second.dBody, b->second.orientation);
		dBodySetLinearVel(b->second.dBody, 0.00, 0.00, 0.00);
		dBodySetAngularVel(b->second.dBody, 0.00, 0.00, 0.00);

		b->second.freeze.position[0] = b->second.position[0];
		b->second.freeze.position[1] = b->second.position[1];
		b->second.freeze.position[2] = b->second.position[2];

		b->second.freeze.orientation[0] = b->second.orientation[0];
		b->second.freeze.orientation[1] = b->second.orientation[1];
		b->second.freeze.orientation[2] = b->second.orientation[2];
		b->second.freeze.orientation[3] = b->second.orientation[3];

		b->second.freeze.linearVel[0] = 0.00;
		b->second.freeze.linearVel[1] = 0.00;
		b->second.freeze.linearVel[2] = 0.00;

		b->second.freeze.angularVel[0] = 0.00;
		b->second.freeze.angularVel[1] = 0.00;
		b->second.freeze.angularVel[2] = 0.00;

		++b;
	}
	
	std::map<std::string, JointData>::iterator j = Joint.begin();

	while (j != Joint.end()) {
		dBodySetPosition(
			j->second.dBody,
			j->second.position[0],
			j->second.position[1],
			j->second.position[2]);
		dBodySetQuaternion(j->second.dBody, j->second.orientation);
		dBodySetLinearVel(j->second.dBody, 0.00, 0.00, 0.00);
		dBodySetAngularVel(j->second.dBody, 0.00, 0.00, 0.00);

		j->second.freeze.position[0] = j->second.position[0];
		j->second.freeze.position[1] = j->second.position[1];
		j->second.freeze.position[2] = j->second.position[2];

		j->second.freeze.orientation[0] = j->second.orientation[0];
		j->second.freeze.orientation[1] = j->second.orientation[1];
		j->second.freeze.orientation[2] = j->second.orientation[2];
		j->second.freeze.orientation[3] = j->second.orientation[3];

		j->second.freeze.linearVel[0] = 0.00;
		j->second.freeze.linearVel[1] = 0.00;
		j->second.freeze.linearVel[2] = 0.00;

		j->second.freeze.angularVel[0] = 0.00;
		j->second.freeze.angularVel[1] = 0.00;
		j->second.freeze.angularVel[2] = 0.00;

		++j;
	}

	UpdateFreeze();
}

void DrawSim()
{
		std::map<std::string, ObjectData>::iterator o = Object.begin();
		while (o != Object.end()) {	
			dQuaternion dQ;
			dRtoQ(dBodyGetRotation(o->second.dBody), dQ);

			float angle;
			Vector3 axis;
			Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
			QuaternionToAxisAngle(q, &axis, &angle);
			const dReal *pos = dBodyGetPosition(o->second.dBody);
			rlPushMatrix();
			rlTranslatef(pos[0], pos[1], pos[2]);
			rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

			switch(o->second.shape)
			{
				case Box: {
					if (o->second.isStatic)
						DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.sides[0], o->second.sides[1], o->second.sides[2], StaticObjectColor);
					else
						DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.sides[0], o->second.sides[1], o->second.sides[2], DynamicObjectColor);
					DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.sides[0], o->second.sides[1], o->second.sides[2], BLACK);
				} break;
				case Sphere: {
					if (o->second.isStatic)
						DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.radius, StaticObjectColor);
					else
						DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.radius, DynamicObjectColor);
					DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.radius, 16, 16, BLACK);
				} break;
				case Capsule: {
					if (o->second.isStatic)
						DrawCapsule((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, 16, 16, StaticObjectColor);
					else
						DrawCapsule((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, 16, 16, DynamicObjectColor);
					DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, 16, 16, BLACK);
				} break;
				case Cylinder: {
					if (o->second.isStatic)
						DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, StaticObjectColor);
					else
						DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, DynamicObjectColor);
					DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, BLACK);
				} break;
			}

			rlPopMatrix();

			++o;
		}
		
		std::map<std::string, BodyData>::iterator b = Body.begin();
		while (b != Body.end()) {	
			dQuaternion dQ;
			dRtoQ(dBodyGetRotation(b->second.dBody), dQ);

			float angle;
			Vector3 axis;
			Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
			QuaternionToAxisAngle(q, &axis, &angle);
			const dReal *pos = dBodyGetPosition(b->second.dBody);
			rlPushMatrix();
			rlTranslatef(pos[0], pos[1], pos[2]);
			rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

			switch(b->second.shape)
			{
				case Box: {
					DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, b->second.sides[0], b->second.sides[1], b->second.sides[2], BodyColor);
					DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, b->second.sides[0], b->second.sides[1], b->second.sides[2], BLACK);
				} break;
				case Sphere: {
					DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, b->second.radius, BodyColor);
					DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, b->second.radius, 16, 16, BLACK);
				} break;
				case Capsule: {
					DrawCapsule((Vector3){ 0.0f, 0.0f, -(b->second.length/2) }, (Vector3){ 0.0f, 0.0f, (b->second.length/2) }, b->second.radius, 16, 16, BodyColor);
					DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(b->second.length/2) }, (Vector3){ 0.0f, 0.0f, (b->second.length/2) }, b->second.radius, 16, 16, BLACK);
				} break;
				case Cylinder: {
					DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, BodyColor);
					DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, BLACK);
				} break;
			}

			rlPopMatrix();

			++b;
		}

		std::map<std::string, JointData>::iterator j = Joint.begin();
		while (j != Joint.end()) {	
			dQuaternion dQ;
			dRtoQ(dBodyGetRotation(j->second.dBody), dQ);

			float angle;
			Vector3 axis;
			Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
			QuaternionToAxisAngle(q, &axis, &angle);
			const dReal *pos = dBodyGetPosition(j->second.dBody);
			rlPushMatrix();
			rlTranslatef(pos[0], pos[1], pos[2]);
			rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

			switch(j->second.shape)
			{
				case Box: {
					DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, j->second.sides[0], j->second.sides[1], j->second.sides[2], JointColor);
					DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, j->second.sides[0], j->second.sides[1], j->second.sides[2], BLACK);
				} break;
				case Sphere: {
					DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, j->second.radius, JointColor);
					DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, j->second.radius, 16, 16, BLACK);
				} break;
				case Capsule: {
					DrawCapsule((Vector3){ 0.0f, 0.0f, -(j->second.length/2) }, (Vector3){ 0.0f, 0.0f, (j->second.length/2) }, j->second.radius, 16, 16, JointColor);
					DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(j->second.length/2) }, (Vector3){ 0.0f, 0.0f, (j->second.length/2) }, j->second.radius, 16, 16, BLACK);
				} break;
				case Cylinder: {
					DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, JointColor);
					DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, BLACK);
				} break;
			}

			rlPopMatrix();

			++j;
		}
}

void DrawGhost()
{
	std::map<std::string, ObjectData>::iterator o = Object.begin();
	while (o != Object.end()) {	
			dQuaternion dQ;
			dRtoQ(dBodyGetRotation(o->second.dBody), dQ);

			float angle;
			Vector3 axis;
			Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
			QuaternionToAxisAngle(q, &axis, &angle);
			const dReal *pos = dBodyGetPosition(o->second.dBody);
			rlPushMatrix();
			rlTranslatef(pos[0], pos[1], pos[2]);
			rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

			switch(o->second.shape)
			{
				case Box: {
					DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.sides[0], o->second.sides[1], o->second.sides[2], GhostColor);
					DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.sides[0], o->second.sides[1], o->second.sides[2], BLACK);
				} break;
				case Sphere: {
					DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.radius, GhostColor);
					DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.radius, 16, 16, BLACK);
				} break;
				case Capsule: {
					DrawCapsule((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, 16, 16, GhostColor);
					DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, 16, 16, BLACK);
				} break;
				case Cylinder: {
					DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, GhostColor);
					DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, BLACK);
				} break;
			}

			rlPopMatrix();

			++o;
		}
		
		std::map<std::string, BodyData>::iterator b = Body.begin();
		while (b != Body.end()) {	
			dQuaternion dQ;
			dRtoQ(dBodyGetRotation(b->second.dBody), dQ);

			float angle;
			Vector3 axis;
			Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
			QuaternionToAxisAngle(q, &axis, &angle);
			const dReal *pos = dBodyGetPosition(b->second.dBody);
			rlPushMatrix();
			rlTranslatef(pos[0], pos[1], pos[2]);
			rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

			switch(b->second.shape)
			{
				case Box: {
					DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, b->second.sides[0], b->second.sides[1], b->second.sides[2], GhostColor);
					DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, b->second.sides[0], b->second.sides[1], b->second.sides[2], BLACK);
				} break;
				case Sphere: {
					DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, b->second.radius, GhostColor);
					DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, b->second.radius, 16, 16, BLACK);
				} break;
				case Capsule: {
					DrawCapsule((Vector3){ 0.0f, 0.0f, -(b->second.length/2) }, (Vector3){ 0.0f, 0.0f, (b->second.length/2) }, b->second.radius, 16, 16, GhostColor);
					DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(b->second.length/2) }, (Vector3){ 0.0f, 0.0f, (b->second.length/2) }, b->second.radius, 16, 16, BLACK);
				} break;
				case Cylinder: {
					DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, GhostColor);
					DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, BLACK);
				} break;
			}

			rlPopMatrix();

			++b;
		}

		std::map<std::string, JointData>::iterator j = Joint.begin();
		while (j != Joint.end()) {	
			dQuaternion dQ;
			dRtoQ(dBodyGetRotation(j->second.dBody), dQ);

			float angle;
			Vector3 axis;
			Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
			QuaternionToAxisAngle(q, &axis, &angle);
			const dReal *pos = dBodyGetPosition(j->second.dBody);
			rlPushMatrix();
			rlTranslatef(pos[0], pos[1], pos[2]);
			rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

			switch(j->second.shape)
			{
				case Box: {
					DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, j->second.sides[0], j->second.sides[1], j->second.sides[2], GhostColor);
					DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, j->second.sides[0], j->second.sides[1], j->second.sides[2], BLACK);
				} break;
				case Sphere: {
					DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, j->second.radius, GhostColor);
					DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, j->second.radius, 16, 16, BLACK);
				} break;
				case Capsule: {
					// WTF is with the capsules hitbox
					DrawCapsule((Vector3){ 0.0f, 0.0f, -(j->second.length/2) }, (Vector3){ 0.0f, 0.0f, (j->second.length/2) }, j->second.radius, 16, 16, GhostColor);
					DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(j->second.length/2) }, (Vector3){ 0.0f, 0.0f, (j->second.length/2) }, j->second.radius, 16, 16, BLACK);
				} break;
				case Cylinder: {
					DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, GhostColor);
					DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, BLACK);
				} break;
			}

			rlPopMatrix();

			++j;
		}
}

void DrawFreeze()
{
	std::map<std::string, ObjectData>::iterator o = Object.begin();
	while (o != Object.end()) {	
			//dQuaternion dQ;
			//dRtoQ(dBodyGetRotation(o->second.dBody), dQ);

			float angle;
			Vector3 axis;
			Quaternion q = {
				o->second.freeze.orientation[1],
				o->second.freeze.orientation[2],
				o->second.freeze.orientation[3],
				o->second.freeze.orientation[0]
			};
			QuaternionToAxisAngle(q, &axis, &angle);
			//const dReal *pos = dBodyGetPosition(o->second.dBody);
			rlPushMatrix();
			rlTranslatef(
				o->second.freeze.position[0],
				o->second.freeze.position[1],
				o->second.freeze.position[2]
			);
			rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

			switch(o->second.shape)
			{
				case Box: {
					if (o->second.isStatic)
						DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.sides[0], o->second.sides[1], o->second.sides[2], StaticObjectColor);
					else
						DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.sides[0], o->second.sides[1], o->second.sides[2], DynamicObjectColor);
					DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.sides[0], o->second.sides[1], o->second.sides[2], BLACK);
				} break;
				case Sphere: {
					if (o->second.isStatic)
						DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.radius, StaticObjectColor);
					else
						DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.radius, DynamicObjectColor);
					DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, o->second.radius, 16, 16, BLACK);
				} break;
				case Capsule: {
					if (o->second.isStatic)
						DrawCapsule((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, 16, 16, StaticObjectColor);
					else
						DrawCapsule((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, 16, 16, DynamicObjectColor);
					DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, 16, 16, BLACK);
				} break;
				case Cylinder: {
					if (o->second.isStatic)
						DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, StaticObjectColor);
					else
						DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, DynamicObjectColor);
					DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, BLACK);
				} break;
			}

			rlPopMatrix();

			++o;
		}
		
		std::map<std::string, BodyData>::iterator b = Body.begin();
		while (b != Body.end()) {	
			//dQuaternion dQ;
			//dRtoQ(dBodyGetRotation(b->second.dBody), dQ);

			float angle;
			Vector3 axis;
			Quaternion q = {
				b->second.freeze.orientation[1],
				b->second.freeze.orientation[2],
				b->second.freeze.orientation[3],
				b->second.freeze.orientation[0]
			};
			QuaternionToAxisAngle(q, &axis, &angle);
			//const dReal *pos = dBodyGetPosition(b->second.dBody);
			rlPushMatrix();
			rlTranslatef(
				b->second.freeze.position[0],
				b->second.freeze.position[1],
				b->second.freeze.position[2]
			);
			rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

			switch(b->second.shape)
			{
				case Box: {
					DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, b->second.sides[0], b->second.sides[1], b->second.sides[2], BodyColor);
					DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, b->second.sides[0], b->second.sides[1], b->second.sides[2], BLACK);
				} break;
				case Sphere: {
					DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, b->second.radius, BodyColor);
					DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, b->second.radius, 16, 16, BLACK);
				} break;
				case Capsule: {
					DrawCapsule((Vector3){ 0.0f, 0.0f, -(b->second.length/2) }, (Vector3){ 0.0f, 0.0f, (b->second.length/2) }, b->second.radius, 16, 16, BodyColor);
					DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(b->second.length/2) }, (Vector3){ 0.0f, 0.0f, (b->second.length/2) }, b->second.radius, 16, 16, BLACK);
				} break;
				case Cylinder: {
					DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, BodyColor);
					DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, BLACK);
				} break;
			}

			rlPopMatrix();

			++b;
		}

		std::map<std::string, JointData>::iterator j = Joint.begin();
		while (j != Joint.end()) {	
			//dQuaternion dQ;
			//dRtoQ(dBodyGetRotation(j->second.dBody), dQ);

			float angle;
			Vector3 axis;
			Quaternion q = {
				j->second.freeze.orientation[1],
				j->second.freeze.orientation[2],
				j->second.freeze.orientation[3],
				j->second.freeze.orientation[0]
			};
			QuaternionToAxisAngle(q, &axis, &angle);
			//const dReal *pos = dBodyGetPosition(j->second.dBody);
			rlPushMatrix();
			rlTranslatef(
				j->second.freeze.position[0],
				j->second.freeze.position[1],
				j->second.freeze.position[2]
			);
			rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

			switch(j->second.shape)
			{
				case Box: {
					DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, j->second.sides[0], j->second.sides[1], j->second.sides[2], JointColor);
					DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, j->second.sides[0], j->second.sides[1], j->second.sides[2], BLACK);
				} break;
				case Sphere: {
					if (j->second.name == SelectedJoint)
						DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, j->second.radius, RED);
					else
						DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, j->second.radius, JointColor);
					DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, j->second.radius, 16, 16, BLACK);
				} break;
				case Capsule: {
					// WTF is with the capsules hitbox
					DrawCapsule((Vector3){ 0.0f, 0.0f, -(j->second.length/2) }, (Vector3){ 0.0f, 0.0f, (j->second.length/2) }, j->second.radius, 16, 16, JointColor);
					DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(j->second.length/2) }, (Vector3){ 0.0f, 0.0f, (j->second.length/2) }, j->second.radius, 16, 16, BLACK);
				} break;
				case Cylinder: {
					DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, JointColor);
					DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(o->second.length/2) }, (Vector3){ 0.0f, 0.0f, (o->second.length/2) }, o->second.radius, o->second.radius, 16, BLACK);
				} break;
			}

			rlPopMatrix();

			++j;
		}
}


void ReFreeze()
{
	game.freeze_t = 0;

	std::map<std::string, ObjectData>::iterator o = Object.begin();

	while (o != Object.end()) {
		dBodySetPosition(
			o->second.dBody,
			o->second.freeze.position[0],
			o->second.freeze.position[1],
			o->second.freeze.position[2]);
		dBodySetQuaternion(o->second.dBody, o->second.freeze.orientation);
		dBodySetLinearVel(
			o->second.dBody,
			o->second.freeze.linearVel[0],
			o->second.freeze.linearVel[1],
			o->second.freeze.linearVel[2]);
		dBodySetAngularVel(
			o->second.dBody,
			o->second.freeze.angularVel[0],
			o->second.freeze.angularVel[1],
			o->second.freeze.angularVel[2]);

		++o;
	}

	std::map<std::string, BodyData>::iterator b = Body.begin();

	while (b != Body.end()) {
		dBodySetPosition(
			b->second.dBody,
			b->second.freeze.position[0],
			b->second.freeze.position[1],
			b->second.freeze.position[2]);
		dBodySetQuaternion(b->second.dBody, b->second.freeze.orientation);
		dBodySetLinearVel(
			b->second.dBody,
			b->second.freeze.linearVel[0],
			b->second.freeze.linearVel[1],
			b->second.freeze.linearVel[2]);
		dBodySetAngularVel(
			b->second.dBody,
			b->second.freeze.angularVel[0],
			b->second.freeze.angularVel[1],
			b->second.freeze.angularVel[2]);

		++b;
	}

	std::map<std::string, JointData>::iterator j = Joint.begin();

	while (j != Joint.end()) {
		dBodySetPosition(
			j->second.dBody,
			j->second.freeze.position[0],
			j->second.freeze.position[1],
			j->second.freeze.position[2]);
		dBodySetQuaternion(j->second.dBody, j->second.freeze.orientation);
		dBodySetLinearVel(
			j->second.dBody,
			j->second.freeze.linearVel[0],
			j->second.freeze.linearVel[1],
			j->second.freeze.linearVel[2]);
		dBodySetAngularVel(
			j->second.dBody,
			j->second.freeze.angularVel[0],
			j->second.freeze.angularVel[1],
			j->second.freeze.angularVel[2]);

		++j;
	}
}

void DrawGameFrame()
{
	if (game.isFrozen)
	{
		DrawFreeze();
		DrawGhost();

		if (game.freeze_t < game.freeze_time)
			++game.freeze_t;
		else
			ReFreeze();
	}
	else
	{
		DrawSim();

		if (game.unfreeze_t < game.unfreeze_time) {
			++game.unfreeze_t;
			++game.game_frame;
		} else {
			UpdateFreeze();
		}
	}
}

void GameEnd()
{
	CloseWindow();
	dJointGroupDestroy(game.contactgroup);
	dSpaceDestroy(game.space);
	dCloseODE();
}

void GameStep(int frame_count)
{
	game.unfreeze_time = frame_count;
	game.isFrozen = false;
	ReFreeze();
}

void GlobalPassiveStateToggle()
{
	ReFreeze();

	GlobalPassiveState = GlobalPassiveState == false;

	dReal s = 0.0f;

	std::map<std::string, JointData>::iterator j = Joint.begin();

	while (j != Joint.end()) {
		if (GlobalPassiveState) s = 100.0f * j->second.strength;
		switch(j->second.connectionType) {
			case Hinge: {
				dJointSetHingeParam(j->second.dJoint[0], dParamFMax, s);
				dJointSetHingeParam(j->second.dJoint[0], dParamVel, 0.0f);
			} break;
			case Slider: {
				dJointSetSliderParam(j->second.dJoint[0], dParamFMax, s);
				dJointSetSliderParam(j->second.dJoint[0], dParamVel, 0.0f);
			} break;
			case Universal: {
				dJointSetUniversalParam(j->second.dJoint[0], dParamFMax, s);
				dJointSetUniversalParam(j->second.dJoint[0], dParamVel, 0.0f);
				dJointSetUniversalParam(j->second.dJoint[0], dParamFMax2, s);
				dJointSetUniversalParam(j->second.dJoint[0], dParamVel2, 0.0f);
			} break;
			case Hinge2: {
				dJointSetHinge2Param(j->second.dJoint[0], dParamFMax, s);
				dJointSetHinge2Param(j->second.dJoint[0], dParamVel, 0.0f);
				dJointSetHinge2Param(j->second.dJoint[0], dParamFMax2, s);
				dJointSetHinge2Param(j->second.dJoint[0], dParamVel2, 0.0f);
			} break;
		}
		++j;
	}
}

void PassiveStateToggle()
{
	if (SelectedJoint == "NONE") return;

	ReFreeze();

	Joint[SelectedJoint].passiveState = Joint[SelectedJoint].passiveState == false;
	
	dReal s = 100.0f * Joint[SelectedJoint].strength;

	if (!Joint[SelectedJoint].passiveState) s = 0.0f;

	switch(Joint[SelectedJoint].connectionType) {
		case Hinge: {
			dJointSetHingeParam(Joint[SelectedJoint].dJoint[0], dParamFMax, s);
			dJointSetHingeParam(Joint[SelectedJoint].dJoint[0], dParamVel, 0.0f);
		} break;
		case Slider: {
			dJointSetSliderParam(Joint[SelectedJoint].dJoint[0], dParamFMax, s);
			dJointSetSliderParam(Joint[SelectedJoint].dJoint[0], dParamVel, 0.0f);
		} break;
		case Universal: {
			dJointSetUniversalParam(Joint[SelectedJoint].dJoint[0], dParamFMax, s);
			dJointSetUniversalParam(Joint[SelectedJoint].dJoint[0], dParamVel, 0.0f);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(Joint[SelectedJoint].dJoint[0], dParamFMax, s);
			dJointSetHinge2Param(Joint[SelectedJoint].dJoint[0], dParamVel, 0.0f);
		} break;
	}
}

void AltPassiveStateToggle()
{
	if (SelectedJoint == "NONE") return;

	ReFreeze();

	Joint[SelectedJoint].altPassiveState = Joint[SelectedJoint].altPassiveState == false;
	
	dReal s = 100.0f * Joint[SelectedJoint].altStrength;

	if (!Joint[SelectedJoint].altPassiveState) s = 0.0f;

	switch(Joint[SelectedJoint].connectionType)
	{
		case Hinge: {
		} break;
		case Slider: {
		} break;
		case Universal: {
			dJointSetUniversalParam(Joint[SelectedJoint].dJoint[0], dParamFMax2, s);
			dJointSetUniversalParam(Joint[SelectedJoint].dJoint[0], dParamVel2, 0.0f);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(Joint[SelectedJoint].dJoint[0], dParamFMax2, s);
			dJointSetHinge2Param(Joint[SelectedJoint].dJoint[0], dParamVel2, 0.0f);
		} break;
	}
}

void ActiveStateToggle()
{
	if (SelectedJoint == "NONE") return;

	ReFreeze();

	Joint[SelectedJoint].activeState = Joint[SelectedJoint].activeState == false;

	dReal dir = 1.0f;

	if (!Joint[SelectedJoint].activeState) dir = -1.0f;
	
	switch(Joint[SelectedJoint].connectionType) {
		case Hinge: {
			dJointSetHingeParam(Joint[SelectedJoint].dJoint[0], dParamFMax, Joint[SelectedJoint].strength);
			dJointSetHingeParam(Joint[SelectedJoint].dJoint[0], dParamVel, dir * Joint[SelectedJoint].velocity);
		} break;
		case Slider: {
			dJointSetSliderParam(Joint[SelectedJoint].dJoint[0], dParamFMax, Joint[SelectedJoint].strength);
			dJointSetSliderParam(Joint[SelectedJoint].dJoint[0], dParamVel, dir * Joint[SelectedJoint].velocity);
		} break;
		case Universal: {
			dJointSetUniversalParam(Joint[SelectedJoint].dJoint[0], dParamFMax, Joint[SelectedJoint].strength);
			dJointSetUniversalParam(Joint[SelectedJoint].dJoint[0], dParamVel, dir * Joint[SelectedJoint].velocity);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(Joint[SelectedJoint].dJoint[0], dParamFMax, Joint[SelectedJoint].strength);
			dJointSetHinge2Param(Joint[SelectedJoint].dJoint[0], dParamVel, dir * Joint[SelectedJoint].velocity);
		} break;
	}
}

void AltActiveStateToggle()
{
	if (SelectedJoint == "NONE") return;

	ReFreeze();
	
	Joint[SelectedJoint].altActiveState = Joint[SelectedJoint].altActiveState == false;

	dReal dir = 1.0f;
	
	if (!Joint[SelectedJoint].altActiveState) dir = -1.0f;
	
	switch(Joint[SelectedJoint].connectionType) {
		case Hinge: {
		} break;
		case Slider: {
		} break;
		case Universal: {
			dJointSetUniversalParam(Joint[SelectedJoint].dJoint[0], dParamFMax2, Joint[SelectedJoint].altStrength);
			dJointSetUniversalParam(Joint[SelectedJoint].dJoint[0], dParamVel2, dir * Joint[SelectedJoint].altVelocity);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(Joint[SelectedJoint].dJoint[0], dParamFMax2, Joint[SelectedJoint].altStrength);
			dJointSetHinge2Param(Joint[SelectedJoint].dJoint[0], dParamVel2, dir * Joint[SelectedJoint].altVelocity);
		} break;
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

void ReplaySave()
{
	//FrameData Frames[game.game_frame/game.turn_frame]
}

int main()
{
	SetTraceLogLevel(LOG_ERROR);

	InitWindow(1280, 720, "Game");

	Camera3D camera = { 0 };
	camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	Vector3 CameraZoom = (Vector3){ 0.0f, 0.0f, 0.0f };
	Vector3 CameraOffset = (Vector3){ 0.0f, -5.0f, 0.0f };

	SetTargetFPS(60);
	
	GameStart();

	Ray ray = { 0 };
	RayCollision c = { 0 };

	while (!WindowShouldClose()) {
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		if (game.isFrozen) {
			std::map<std::string, BodyData>::iterator b = Body.begin();

			while (b != Body.end()) {
				x = x + b->second.freeze.position[0];
				y = y + b->second.freeze.position[1];
				z = z + b->second.freeze.position[2];
				++b;
			}
			x = x/Body.size();
			y = y/Body.size();
			z = z/Body.size();
			camera.target = (Vector3){ x, y, z };
			camera.position = (Vector3){
				x + CameraOffset.x,
				y + CameraOffset.y,
				z + CameraOffset.z
			};
		} else {
			std::map<std::string, BodyData>::iterator b = Body.begin();

			while (b != Body.end()) {
				const dReal *position = dBodyGetPosition(b->second.dBody);
				x = x + position[0];
				y = y + position[1];
				z = z + position[2];
				++b;
			}
			x = x/Body.size();
			y = y/Body.size();
			z = z/Body.size();
			camera.target = (Vector3){ x, y, z };
			camera.position = (Vector3){
				x + CameraOffset.x,
				y + CameraOffset.y,
				z + CameraOffset.z
			};
		}
		
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

		if (IsKeyPressed(KEY_P))
			PhysicsPaused = PhysicsPaused == false;

		if (IsKeyPressed(KEY_R))
			GameReset();
		
		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			if (IsKeyPressed(KEY_Z))
				AltActiveStateToggle();

			if (IsKeyPressed(KEY_X))
				AltPassiveStateToggle();
		} else {
			if (IsKeyPressed(KEY_Z))
				ActiveStateToggle();

			if (IsKeyPressed(KEY_X))
				PassiveStateToggle();
		}

		if (IsKeyPressed(KEY_C))
			GlobalPassiveStateToggle();

		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			if (IsKeyPressed(KEY_SPACE))
				if (game.isFrozen) GameStep(1);
		} else {
			if (IsKeyPressed(KEY_SPACE))
				if (game.isFrozen) GameStep(game.turn_frame);
		}

		//TODO: Fix joint selection
		ray = GetMouseRay(GetMousePosition(), camera);
		
		std::map<std::string, JointData>::iterator j = Joint.begin();

		while (j != Joint.end()) {
		  c = GetRayCollisionSphere(ray,
				(Vector3){
					j->second.freeze.position[0],
					j->second.freeze.position[1],
					j->second.freeze.position[2]
				},
				j->second.radius);

			if (!c.hit)
				SelectedJoint = "NONE";
			else {
				SelectedJoint = j->second.name;
				break;
			}

			++j;
		}

		if (!PhysicsPaused) {
			dSpaceCollide(game.space, 0, &nearCallback);
			dWorldStep(game.world, game.step);
			dJointGroupEmpty(game.contactgroup);
		}
		
		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
				DrawGameFrame();
				if (game.globalplane.state == true) {
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
			EndMode3D();
			DrawText(TextFormat("%d", game.game_frame), 640, 20, 32, BLACK);
		EndDrawing();
	}
	
	GameEnd();

	return 0;
}
