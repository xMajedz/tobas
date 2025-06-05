#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <lua.h>
#include <lualib.h>
#include <luacode.h>

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

#include <ode/ode.h>

struct Bytecode {
	size_t size;
	char* data;
};

char* readfile(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* content = (char*) malloc(fsize + 1);
	fread(content, fsize, 1, f);
	content[fsize] = '\0';
	fclose(f);
	return content;
}

int luau_do(lua_State* L, const char* string, const char* chunkname)
{
	Bytecode bytecode = { 0 };
	bytecode.data = luau_compile(string, strlen(string), NULL, &bytecode.size);
	int result = luau_load(L, chunkname, bytecode.data, bytecode.size, 0);
	free(bytecode.data);
	return lua_pcall(L, 0, 0, 0);
}

int luau_dostring(lua_State* L, const char* string)
{
  return luau_do(L, string, "=dostring");
}

int luau_dostring(lua_State* L, const char* string, const char* chunkname)
{
	return luau_do(L, string, chunkname);
}

int luau_dofile(lua_State* L, const char* filename)
{
	return luau_dostring(L, readfile(filename), "=dofile");
}

int luau_dofile(lua_State* L, const char* filename, const char* chunkname)
{
	return luau_dostring(L, readfile(filename), chunkname);
}

void lua_setglobalcfunction(lua_State* L, lua_CFunction fn ,const char* fn_name)
{
	lua_pushcfunction(L, fn, fn_name);
	lua_setglobal(L, fn_name);
}

int api_readfile(lua_State* L)
{
	const char* filename = lua_tostring(L, -1);
	const char* result = readfile(filename);
	lua_pushstring(L, result);
	return 1;
}

int api_dofile(lua_State* L)
{
	const char* filename = lua_tostring(L, -1);
	lua_Number result = luau_dofile(L, filename);
	lua_pushnumber(L, result);
	return 1;
}

enum GameContext {
	NoContext,
	ObjectContext,
	PlayerContext,
	BodyContext,
	JointContext
} DataContext = NoContext;

enum Shape {
	Box,
	Sphere,
	Capsule,
	Cylinder,
	Composite,
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
	int numplayers = 1;
	Gamemode gamemode = FREEPLAY;
	std::string selected_player = "NONE";
	std::string selected_joint = "NONE";
	bool freeze;
	bool pause = false;
} game;

class FreezeData {
	public:
	dReal position[3];
	dReal orientation[4];
	dReal angular_vel[3];
	dReal linear_vel[3];
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
	FreezeData freeze;
	bool static_state;
	Color color;
	Color ghost_color;
	Color select_color;
	bool select;
	bool ghost;

	unsigned long category_bits;
	unsigned long collide_bits;

	Object() {
		ghost_color = (Color){51, 51, 51, 51};
	};

	void make_static() {
		if (static_state) {
			dJointID fixed = dJointCreateFixed(game.world ,0);
			dJointAttach(fixed, dBody, 0);
			dJointSetFixed(fixed);
		}
	};

	void create(dMass mass) {
		dBody = dBodyCreate(game.world);
		dBodySetPosition(
			dBody,
			position[0],
			position[1],
			position[2]
		);

		dBodySetQuaternion(dBody, orientation);
		
		switch(shape) {
			case Box: {
				dGeom = dCreateBox(game.space, sides[0], sides[1], sides[2]);
				dMassSetBox(&mass, density, sides[0], sides[1], sides[2]);
			} break;
			case Sphere: {
				dGeom = dCreateSphere(game.space, radius);
				dMassSetSphere(&mass, density, radius);
			} break;
			case Capsule: {
				dGeom = dCreateCapsule(game.space, radius, length);
				dMassSetCapsule(&mass, density, 1, length, radius);
			} break;
			case Cylinder: {
				dGeom = dCreateCylinder(game.space, radius, length);
				dMassSetCylinder(&mass, density, 1, length, radius);
			} break;
		}

		dMassAdjust(&mass, 0.5);
		dBodySetMass(dBody, &mass);
		dGeomSetBody(dGeom, dBody);

		dGeomSetCategoryBits(dGeom, category_bits);
		dGeomSetCollideBits(dGeom, collide_bits);
	};

	void update_freeze() {
		const dReal *linear_vel = dBodyGetLinearVel(dBody);
		const dReal *angular_vel = dBodyGetAngularVel(dBody);
		const dReal *position = dGeomGetPosition(dGeom);
		dQuaternion orientation;
		dGeomGetQuaternion(dGeom, orientation);

		freeze.position[0] = position[0];
		freeze.position[1] = position[1];
		freeze.position[2] = position[2];

		freeze.orientation[0] = orientation[0];
		freeze.orientation[1] = orientation[1];
		freeze.orientation[2] = orientation[2];
		freeze.orientation[3] = orientation[3];

		freeze.linear_vel[0] = linear_vel[0];
		freeze.linear_vel[1] = linear_vel[1];
		freeze.linear_vel[2] = linear_vel[2];

		freeze.angular_vel[0] = angular_vel[0];
		freeze.angular_vel[1] = angular_vel[1];
		freeze.angular_vel[2] = angular_vel[2];
	};

	void refreeze() {
		dGeomSetPosition(
			dGeom,
			freeze.position[0],
			freeze.position[1],
			freeze.position[2]);
		dGeomSetQuaternion(dGeom, freeze.orientation);
		dBodySetLinearVel(
			dBody,
			freeze.linear_vel[0],
			freeze.linear_vel[1],
			freeze.linear_vel[2]);
		dBodySetAngularVel(
			dBody,
			freeze.angular_vel[0],
			freeze.angular_vel[1],
			freeze.angular_vel[2]);
	};

	void draw_object(Color draw_color) {
		switch(shape) {
			case Box: {
				DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, sides[0], sides[1], sides[2], draw_color);
				DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, sides[0], sides[1], sides[2], BLACK);
			} break;
			case Sphere: {
				DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, radius, draw_color);
				DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, radius, 16, 16, BLACK);
			} break;
			case Capsule: {
				DrawCapsule((Vector3){ 0.0f, 0.0f, -(length/2) }, (Vector3){ 0.0f, 0.0f, (length/2) }, radius, 16, 16, draw_color);
				DrawCapsuleWires((Vector3){ 0.0f, 0.0f, -(length/2) }, (Vector3){ 0.0f, 0.0f, (length/2) }, radius, 16, 16, BLACK);
			} break;
			case Cylinder: {
				DrawCylinderEx((Vector3){ 0.0f, 0.0f, -(length/2) }, (Vector3){ 0.0f, 0.0f, (length/2) }, radius, radius, 16, draw_color);
				DrawCylinderWiresEx((Vector3){ 0.0f, 0.0f, -(length/2) }, (Vector3){ 0.0f, 0.0f, (length/2) }, radius, radius, 16, BLACK);
			} break;
		}
		rlPopMatrix();
	}

	void draw(Color draw_color) {
		dQuaternion dQ;
		dRtoQ(dGeomGetRotation(dGeom), dQ);

		float angle;
		Vector3 axis;
		Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
		QuaternionToAxisAngle(q, &axis, &angle);
		const dReal *pos = dGeomGetPosition(dGeom);
		rlPushMatrix();
		rlTranslatef(pos[0], pos[1], pos[2]);
		rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

		draw_object(draw_color);
	};

	void draw_freeze() {
		float angle;
		Vector3 axis;
		Quaternion q = {
			freeze.orientation[1],
			freeze.orientation[2],
			freeze.orientation[3],
			freeze.orientation[0]
		};
		QuaternionToAxisAngle(q, &axis, &angle);
		rlPushMatrix();
		rlTranslatef(
			freeze.position[0],
			freeze.position[1],
			freeze.position[2]
		);
		rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);
		
		if (select) {
			draw_object(select_color);
		} else {
			draw_object(color);
		}
	};

	void draw_ghost() {
		if (ghost) {
			draw(ghost_color);
		}
	};

	void draw() {
		draw(color);
	};

	void toggle_ghost() {
		ghost = ghost == false;
	};

	RayCollision collide_mouse_ray(Ray ray, RayCollision collision) {
		collision = GetRayCollisionSphere(ray,
			(Vector3){
				freeze.position[0],
				freeze.position[1],
				freeze.position[2],
			},
			radius
		);
		return collision;
	};

};

class Body : public Object {
	public:
	std::string owner;
};

class Joint : public Object {
	public:
	std::string owner;
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

	void create_joint(dBodyID b1, dBodyID b2) {
		switch(connectionType) {
			case Hinge: {
				dJoint[0] = dJointCreateHinge(game.world, 0);
				dJointAttach(dJoint[0], b1, dBody);
				dJointSetHingeAnchor(
					dJoint[0],
					position[0],
					position[1],
					position[2]);
				dJointSetHingeAxis(
					dJoint[0],
					axis[0],
					axis[1],
					axis[2]);
		
				dJointSetHingeParam(
					dJoint[0],
					dParamHiStop,
					range[0]);
				dJointSetHingeParam(
					dJoint[0],
					dParamLoStop,
					range[1]);
			} break;
			case Slider: {
				dJoint[0] = dJointCreateSlider(game.world, 0);
				dJointAttach(dJoint[0], b1, dBody);
				dJointSetSliderAxis(
					dJoint[0],
					axis[0],
					axis[1],
					axis[2]);
		
				dJointSetSliderParam(
					dJoint[0],
					dParamHiStop,
					range[0]);
				dJointSetSliderParam(
					dJoint[0],
					dParamLoStop,
					range[1]);
			} break;
			case Universal: {
				dJoint[0] = dJointCreateUniversal(game.world, 0);
				dJointAttach(dJoint[0], b1, dBody);
				dJointSetUniversalAnchor(
					dJoint[0],
					position[0],
					position[1],
					position[2]);
		
				dJointSetUniversalAnchor(
					dJoint[0],
					position[0],
					position[1],
					position[2]);
		
				dJointSetUniversalAxis1(
					dJoint[0],
					axis[0],
					axis[1],
					axis[2]);
				dJointSetUniversalAxis2(
					dJoint[0],
					axis_alt[0],
					axis_alt[1],
					axis_alt[2]);
		
				dJointSetUniversalParam(
					dJoint[0],
					dParamHiStop,
					range[0]);
		
				dJointSetUniversalParam(
					dJoint[0],
					dParamHiStop2,
					range_alt[0]);
		
				dJointSetUniversalParam(
					dJoint[0],
					dParamLoStop,
					range[1]);
		
				dJointSetUniversalParam(
					dJoint[0],
					dParamLoStop2,
					range_alt[1]);
			} break;
			case Hinge2: {
				dJoint[0] = dJointCreateHinge2(game.world, 0);
				dJointAttach(dJoint[0], b1, dBody);
				dJointSetHinge2Anchor(
					dJoint[0],
					position[0],
					position[1],
					position[2]);
		
				dJointSetHinge2Anchor(
					dJoint[0],
					position[0],
					position[1],
					position[2]);
			
				dJointSetHinge2Axes(dJoint[0], axis, axis_alt);
		
				dJointSetHinge2Param(
					dJoint[0],
					dParamHiStop,
					range[0]);
		
				dJointSetHinge2Param(
					dJoint[0],
					dParamHiStop2,
					range_alt[0]);
		
				dJointSetHinge2Param(
					dJoint[0],
					dParamLoStop,
					range[1]);
		
				dJointSetHinge2Param(
					dJoint[0],
					dParamLoStop2,
					range_alt[1]);
			} break;
			default:
				dJoint[0] = dJointCreateFixed(game.world, 0);
				dJointAttach(dJoint[0], b1, dBody);
				dJointSetFixed(dJoint[0]);
		}

		dJoint[1] = dJointCreateFixed(game.world, 0);
		dJointAttach(dJoint[1], dBody, b2);
		dJointSetFixed(dJoint[1]);

		dGeomSetCategoryBits(dGeom, category_bits);
		dGeomSetCollideBits(dGeom, collide_bits);
	};
};

enum PlayerPassiveStates {
	HOLD_ALL,
	RELAX_ALL,
	MIXED,
}; 

class Player {
	public:
	std::string name;
	std::map<std::string, Body> body;
	std::map<std::string, Joint> joint;
	PlayerPassiveStates passive_states;
	PlayerPassiveStates passive_states_alt;
	
	dReal engagepos[3];
	dReal engagerot[3];

	bool use_engagepos;
	bool use_engagerot;

	unsigned long body_category_bits;
	unsigned long body_collide_bits;
	unsigned long joint_category_bits;
	unsigned long joint_collide_bits;

	bool ghost;
	Color ghost_color;
	Color joint_color;
	Color joint_select_color;
	Color body_color;

	Player() {
		passive_states = RELAX_ALL;
		passive_states_alt = RELAX_ALL;

		body_color = (Color){ 255, 255, 255, 255 };
		joint_color = BLACK;
		ghost_color = (Color){
			joint_color.r,
			joint_color.g,
			joint_color.b,
			55,
		};
	};

	void create(dMass mass) {
		for (auto& [body_name, b] : body) {
			b.create(mass);
		}
	
		for (auto& [joint_name, j] : joint) {
			j.create(mass);
			j.create_joint(
				body[j.connections[0]].dBody,
				body[j.connections[1]].dBody
			);
		}
	};

	void update_freeze() {
		for (auto& [body_name, b] : body) {
			b.update_freeze();
		}

		for (auto& [joint_name, j] : joint) {
			j.update_freeze();
		}
	};

	void refreeze() {
		for (auto& [body_name, b] : body) {
			b.refreeze();
		}

		for (auto& [joint_name, j] : joint) {
			j.refreeze();
		}
	};

	void draw() {
		for (auto& [body_name, b] : body) {
			b.draw();
		}
	
		for (auto& [joint_name, j] : joint) {
			j.draw();
		}
	};

	void draw_freeze() {
		for (auto& [body_name, b] : body) {
			b.draw_freeze();
		}
	
		for (auto& [joint_name, j] : joint) {
			j.draw_freeze();
		}
	};

	void draw_ghost() {
		if (ghost) {
			for (auto& [body_name, b] : body) {
				b.draw_ghost();
			}
	
			for (auto& [joint_name, j] : joint) {
				j.draw_ghost();
			}
		}
	};

	void toggle_ghost() {
		for (auto& [body_name, b] : body) {
			b.toggle_ghost();
		}
	
		for (auto& [joint_name, j] : joint) {
			j.toggle_ghost();
		}
	};
};

class FrameData : public FreezeData {
	public:
	std::map<std::string, Player> player;
};

std::string MSG;

std::map<std::string, Object> object;
std::map<std::string, Player> player;

Color DynamicObjectColor = (Color){ 0, 255, 0, 255 };
Color StaticObjectColor = (Color){ 51, 51, 51, 255 };

std::string object_key;
std::string player_key;
std::string body_key;
std::string joint_key;

unsigned long StaticObjectCategoryBits = 0b0001;
unsigned long StaticObjectCollideBits = 0b0000;

unsigned long DynamicObjectCategoryBits = 0b0001;
unsigned long DynamicObjectCollideBits = 0b0001;

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

int turnframes(lua_State* L)
{
	lua_rawgeti(L, -1, 1);

	lua_Number turnframes = lua_tonumber(L, -1);

	switch(DataContext)
	{
		case NoContext: {
			game.turnframes = turnframes;
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

int numplayers(lua_State* L)
{
	lua_rawgeti(L, -1, 1);

	lua_Number numplayers = lua_tonumber(L, -1); 

	switch(DataContext)
	{
		case NoContext: {
			game.numplayers = numplayers;
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
int friction(lua_State* L)
{
	lua_Number friction;

	lua_rawgeti(L, -1, 1);
	friction = lua_tonumber(L, -1);

	switch(DataContext)
	{
		case NoContext: {
			game.friction = friction;
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

int engagepos(lua_State* L)
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
			player[player_key].engagepos[0] = pos[0];
			player[player_key].engagepos[1] = pos[1];
			player[player_key].engagepos[2] = pos[2];
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

int engagerot(lua_State* L)
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
			player[player_key].engagerot[0] = rot[0];
			player[player_key].engagerot[1] = rot[1];
			player[player_key].engagerot[2] = rot[2];
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

int engageplayerpos(lua_State* L)
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
			game.engageplayerpos[0] = pos[0];
			game.engageplayerpos[1] = pos[1];
			game.engageplayerpos[2] = pos[2];
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

int engageplayerrot(lua_State* L)
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
			game.engageplayerrot[0] = rot[0];
			game.engageplayerrot[1] = rot[1];
			game.engageplayerrot[2] = rot[2];
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
			// Error Handling	
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

int api_object(lua_State* L)
{
	DataContext = ObjectContext;

	std::string name = lua_tostring(L, -1);
	
	object_key = name;

	object[object_key].name = name;

	object[object_key].color = DynamicObjectColor;

	object[object_key].orientation[0] = 1.00;
	object[object_key].orientation[1] = 0.00;
	object[object_key].orientation[2] = 0.00;
	object[object_key].orientation[3] = 0.00;

	object[object_key].freeze.orientation[0] = 1.00;
	object[object_key].freeze.orientation[1] = 0.00;
	object[object_key].freeze.orientation[2] = 0.00;
	object[object_key].freeze.orientation[3] = 0.00;

	object[object_key].freeze.linear_vel[0] = 0.00;
	object[object_key].freeze.linear_vel[1] = 0.00;
	object[object_key].freeze.linear_vel[2] = 0.00;

	object[object_key].freeze.angular_vel[0] = 0.00;
	object[object_key].freeze.angular_vel[1] = 0.00;
	object[object_key].freeze.angular_vel[2] = 0.00;

	object[object_key].category_bits = DynamicObjectCategoryBits;
	object[object_key].collide_bits = DynamicObjectCollideBits;

	object[object_key].select = false;
	object[object_key].static_state = false;

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int api_player(lua_State* L)
{
	DataContext = PlayerContext;

	std::string name = lua_tostring(L, -1);

	if (player.size() > game.numplayers + 1) {
		lua_Number result = 1;
		lua_pushnumber(L, result);
		return 1;
	}

	player_key = name;
	
	player[player_key].name = name;

	switch (player.size()) {
		case 1: {
			game.selected_player = player_key;
			player[player_key].joint_color = MAROON;
			player[player_key].body_category_bits = 1<<2;
			player[player_key].joint_category_bits = 1<<3;
		} break;
		case 2: {
			player[player_key].joint_color = DARKBLUE;
			player[player_key].body_category_bits = 1<<4;
			player[player_key].joint_category_bits = 1<<5;
		} break;
		case 3: {
			player[player_key].joint_color = DARKGREEN;
			player[player_key].body_category_bits = 1<<6;
			player[player_key].joint_category_bits = 1<<7;
		} break;
		case 4: {
			player[player_key].joint_color = DARKPURPLE;
			player[player_key].body_category_bits = 1<<8;
			player[player_key].joint_category_bits = 1<<9;
		} break;
	}

	player[player_key].body_collide_bits = 0b0001; 
	player[player_key].joint_collide_bits = 0b0001;

	player[player_key].ghost = true;
	player[player_key].ghost_color = player[player_key].joint_color;
	player[player_key].ghost_color.a = 55;

	player[player_key].joint_select_color = WHITE;

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int api_body(lua_State* L)
{
	DataContext = BodyContext;

	std::string name = lua_tostring(L, -1);
	
	body_key = name;

	player[player_key].body[body_key].name = name;
	player[player_key].body[body_key].owner = player_key;

	player[player_key].body[body_key].color = player[player_key].body_color;
	player[player_key].body[body_key].ghost_color = player[player_key].ghost_color;

	player[player_key].body[body_key].orientation[0] = 1.00;
	player[player_key].body[body_key].orientation[1] = 0.00;
	player[player_key].body[body_key].orientation[2] = 0.00;
	player[player_key].body[body_key].orientation[3] = 0.00;

	player[player_key].body[body_key].freeze.orientation[0] = 1.00;
	player[player_key].body[body_key].freeze.orientation[1] = 0.00;
	player[player_key].body[body_key].freeze.orientation[2] = 0.00;
	player[player_key].body[body_key].freeze.orientation[3] = 0.00;

	player[player_key].body[body_key].freeze.linear_vel[0] = 0.00;
	player[player_key].body[body_key].freeze.linear_vel[1] = 0.00;
	player[player_key].body[body_key].freeze.linear_vel[2] = 0.00;

	player[player_key].body[body_key].freeze.angular_vel[0] = 0.00;
	player[player_key].body[body_key].freeze.angular_vel[1] = 0.00;
	player[player_key].body[body_key].freeze.angular_vel[2] = 0.00;

	player[player_key].body[body_key].category_bits = player[player_key].body_category_bits;
	player[player_key].body[body_key].collide_bits = player[player_key].body_collide_bits;

	player[player_key].body[body_key].ghost = true;
	player[player_key].body[body_key].static_state = false;

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int api_joint(lua_State* L)
{
	DataContext = JointContext;

	std::string name = lua_tostring(L, -1);

	joint_key = name;

	player[player_key].joint[joint_key].name = name;
	player[player_key].joint[joint_key].owner = player_key;

	player[player_key].joint[joint_key].color = player[player_key].joint_color;
	player[player_key].joint[joint_key].ghost_color = player[player_key].ghost_color;
	player[player_key].joint[joint_key].select_color = player[player_key].joint_select_color;

	player[player_key].joint[joint_key].orientation[0] = 1.00;
	player[player_key].joint[joint_key].orientation[1] = 0.00;
	player[player_key].joint[joint_key].orientation[2] = 0.00;
	player[player_key].joint[joint_key].orientation[3] = 0.00;

	player[player_key].joint[joint_key].freeze.orientation[0] = 1.00;
	player[player_key].joint[joint_key].freeze.orientation[1] = 0.00;
	player[player_key].joint[joint_key].freeze.orientation[2] = 0.00;
	player[player_key].joint[joint_key].freeze.orientation[3] = 0.00;

	player[player_key].joint[joint_key].freeze.linear_vel[0] = 0.00;
	player[player_key].joint[joint_key].freeze.linear_vel[1] = 0.00;
	player[player_key].joint[joint_key].freeze.linear_vel[2] = 0.00;

	player[player_key].joint[joint_key].freeze.angular_vel[0] = 0.00;
	player[player_key].joint[joint_key].freeze.angular_vel[1] = 0.00;
	player[player_key].joint[joint_key].freeze.angular_vel[2] = 0.00;

	player[player_key].joint[joint_key].category_bits = player[player_key].joint_category_bits;
	player[player_key].joint[joint_key].collide_bits = player[player_key].joint_collide_bits;

	player[player_key].joint[joint_key].state = RELAX;
	player[player_key].joint[joint_key].state_alt = RELAX;

	player[player_key].joint[joint_key].ghost = true;
	player[player_key].joint[joint_key].static_state= false;

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
				player[player_key].body[body_key].shape = Box;
			} else if ("sphere" == shape) {
				player[player_key].body[body_key].shape = Sphere;
			} else if ("capsule" == shape) {
				player[player_key].body[body_key].shape = Capsule;
			} else if ("cylinder" == shape) {
				player[player_key].body[body_key].shape = Cylinder;
			} else  {
				//Error Handling
			}
		} break;
		case JointContext: {
			if ("box"  == shape) {
				player[player_key].joint[joint_key].shape = Box;
			} else if ("sphere" == shape) {
				player[player_key].joint[joint_key].shape = Sphere;
			} else if ("capsule" == shape) {
				player[player_key].joint[joint_key].shape = Capsule;
			} else if ("cylinder" == shape) {
				player[player_key].joint[joint_key].shape = Cylinder;
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
	
	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			object[object_key].position[0] = position[0];
			object[object_key].position[1] = position[1];
			object[object_key].position[2] = position[2];
			object[object_key].freeze.position[0] = position[0];
			object[object_key].freeze.position[1] = position[1];
			object[object_key].freeze.position[2] = position[2];
		} break;
		case BodyContext: {
			if (player[player_key].engagepos) {
				position[0] = position[0] + player[player_key].engagepos[0];
				position[1] = position[1] + player[player_key].engagepos[1];
				position[2] = position[2] + player[player_key].engagepos[2];
			}
			player[player_key].body[body_key].position[0] = position[0];
			player[player_key].body[body_key].position[1] = position[1];
			player[player_key].body[body_key].position[2] = position[2];
			player[player_key].body[body_key].freeze.position[0] = position[0];
			player[player_key].body[body_key].freeze.position[1] = position[1];
			player[player_key].body[body_key].freeze.position[2] = position[2];
		} break;
		case JointContext: {
			if (player[player_key].engagepos) {
				position[0] = position[0] + player[player_key].engagepos[0];
				position[1] = position[1] + player[player_key].engagepos[1];
				position[2] = position[2] + player[player_key].engagepos[2];
			}
			player[player_key].joint[joint_key].position[0] = position[0];
			player[player_key].joint[joint_key].position[1] = position[1];
			player[player_key].joint[joint_key].position[2] = position[2];
			player[player_key].joint[joint_key].freeze.position[0] = position[0];
			player[player_key].joint[joint_key].freeze.position[1] = position[1];
			player[player_key].joint[joint_key].freeze.position[2] = position[2];
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
			player[player_key].body[body_key].orientation[0] = orientation[0];
			player[player_key].body[body_key].orientation[1] = orientation[1];
			player[player_key].body[body_key].orientation[2] = orientation[2];
			player[player_key].body[body_key].orientation[3] = orientation[3];

			player[player_key].body[body_key].freeze.orientation[0] = orientation[0];
			player[player_key].body[body_key].freeze.orientation[1] = orientation[1];
			player[player_key].body[body_key].freeze.orientation[2] = orientation[2];
			player[player_key].body[body_key].freeze.orientation[3] = orientation[3];
		} break;
		case JointContext: {
			player[player_key].joint[joint_key].orientation[0] = orientation[0];
			player[player_key].joint[joint_key].orientation[1] = orientation[1];
			player[player_key].joint[joint_key].orientation[2] = orientation[2];
			player[player_key].joint[joint_key].orientation[3] = orientation[3];
	
			player[player_key].joint[joint_key].freeze.orientation[0] = orientation[0];
			player[player_key].joint[joint_key].freeze.orientation[1] = orientation[1];
			player[player_key].joint[joint_key].freeze.orientation[2] = orientation[2];
			player[player_key].joint[joint_key].freeze.orientation[3] = orientation[3];
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
			object[object_key].sides[0] = sides[0];
			object[object_key].sides[1] = sides[1];
			object[object_key].sides[2] = sides[2];
		} break;
		case BodyContext: {
			player[player_key].body[body_key].sides[0] = sides[0];
			player[player_key].body[body_key].sides[1] = sides[1];
			player[player_key].body[body_key].sides[2] = sides[2];
		} break;
		case JointContext: {
			player[player_key].joint[joint_key].sides[0] = sides[0];
			player[player_key].joint[joint_key].sides[1] = sides[1];
			player[player_key].joint[joint_key].sides[2] = sides[2];
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
			object[object_key].density = density;
		} break;
		case BodyContext: {
			player[player_key].body[body_key].density = density;
		} break;
		case JointContext: {
			player[player_key].joint[joint_key].density = density;
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
		case ObjectContext: {
			object[object_key].static_state = true;
			object[object_key].color = StaticObjectColor;
			object[object_key].category_bits = StaticObjectCategoryBits;
			object[object_key].collide_bits = StaticObjectCollideBits;
		} break;
		case BodyContext: {
			player[player_key].body[body_key].static_state = true;
		} break;
		case JointContext: {
			player[player_key].joint[joint_key].static_state = true;
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
			object[object_key].radius = radius;
		} break;
		case BodyContext: {
			player[player_key].body[body_key].radius = radius;
		} break;
		case JointContext: {
			player[player_key].joint[joint_key].radius = radius;
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
			object[object_key].length = length;
		} break;
		case BodyContext: {
			player[player_key].body[body_key].length = length;
		} break;
		case JointContext: {
			player[player_key].joint[joint_key].length = length;
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

	switch(DataContext) {
		case JointContext: {
			player[player_key].joint[joint_key].strength = strength;
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

	switch(DataContext) {
		case JointContext: {
			player[player_key].joint[joint_key].strength_alt = strength_alt;
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
			player[player_key].joint[joint_key].velocity = velocity;
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
			player[player_key].joint[joint_key].velocity_alt = velocity_alt;
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
			player[player_key].joint[joint_key].axis[0] = axis[0];
			player[player_key].joint[joint_key].axis[1] = axis[1];
			player[player_key].joint[joint_key].axis[2] = axis[2];
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
			player[player_key].joint[joint_key].axis_alt[0] = axis_alt[0];
			player[player_key].joint[joint_key].axis_alt[1] = axis_alt[1];
			player[player_key].joint[joint_key].axis_alt[2] = axis_alt[2];
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
			player[player_key].joint[joint_key].range[0] = range[0];
			player[player_key].joint[joint_key].range[1] = range[1];
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
			player[player_key].joint[joint_key].range_alt[0] = range_alt[0];
			player[player_key].joint[joint_key].range_alt[1] = range_alt[1];
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
			player[player_key].joint[joint_key].connections[0] = connections[0];
			player[player_key].joint[joint_key].connections[1] = connections[1];
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
				player[player_key].joint[joint_key].connectionType = Hinge;
			} else if ("slider" == connectionType) {
				player[player_key].joint[joint_key].connectionType = Slider;
			} else if ("universal" == connectionType) {
				player[player_key].joint[joint_key].connectionType = Universal;
			} else if ("hinge2" == connectionType) {
				player[player_key].joint[joint_key].connectionType = Hinge2;
			} else {
			// Error Handling
			}
		} break;
	}

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

// api draw
int api_DrawText(lua_State* L)
{
	Color color;
	lua_rawgeti(L, -1, 1);
	color.r = lua_tonumber(L, -1);
	lua_rawgeti(L, -2, 2);
	color.g = lua_tonumber(L, -1);
	lua_rawgeti(L, -3, 3);
	color.b = lua_tonumber(L, -1);
	lua_rawgeti(L, -4, 4);
	color.a = lua_tonumber(L, -1);

	int fontSize = lua_tointeger(L, -6);
	int posY = lua_tointeger(L, -7);
	int posX = lua_tointeger(L, -8);
	const char* text = lua_tostring(L, -9); 	
	DrawText(text, posX, posY, fontSize, color);

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int api_DrawRectangle(lua_State* L)
{
	Color color;
	lua_rawgeti(L, -1, 1);
	color.r = lua_tonumber(L, -1);
	lua_rawgeti(L, -2, 2);
	color.g = lua_tonumber(L, -1);
	lua_rawgeti(L, -3, 3);
	color.b = lua_tonumber(L, -1);
	lua_rawgeti(L, -4, 4);
	color.a = lua_tonumber(L, -1);

	int height = lua_tointeger(L, -6);
	int width = lua_tointeger(L, -7);
	int posY = lua_tointeger(L, -8);
	int posX = lua_tointeger(L, -9);
	DrawRectangle(posX, posY, width, height, color);

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

int api_DrawRectangleLines(lua_State* L)
{
	Color color;
	lua_rawgeti(L, -1, 1);
	color.r = lua_tonumber(L, -1);
	lua_rawgeti(L, -2, 2);
	color.g = lua_tonumber(L, -1);
	lua_rawgeti(L, -3, 3);
	color.b = lua_tonumber(L, -1);
	lua_rawgeti(L, -4, 4);
	color.a = lua_tonumber(L, -1);

	int height = lua_tointeger(L, -6);
	int width = lua_tointeger(L, -7);
	int posY = lua_tointeger(L, -8);
	int posX = lua_tointeger(L, -9);
	DrawRectangleLines(posX, posY, width, height, color);

	lua_Number result = 1;
	lua_pushnumber(L, result);
	return 1;
}

void api_draw_functions(lua_State* L)
{
	lua_setglobalcfunction(L, api_DrawText, "DrawText");
	lua_setglobalcfunction(L, api_DrawRectangle, "DrawRectangle");
	lua_setglobalcfunction(L, api_DrawRectangleLines, "DrawRectangleLines");
}

void api_draw2d(lua_State* L)
{
	lua_getglobal(L, "draw2d");
	if (lua_isfunction(L, -1)) {
		if (lua_pcall(L, 0, 0, 0) == LUA_OK) {
		}
	}
}

// api state getters

int api_get_gameframe(lua_State* L)
{
	lua_pushnumber(L, game.gameframe);
	return 1;
}

void api_state_getters(lua_State* L)
{
	lua_setglobalcfunction(L, api_get_gameframe, "get_gameframe");
}

// api main
void api_init(lua_State* L)
{	
	lua_setglobalcfunction(L, turnframes, "turnframes");
	lua_setglobalcfunction(L, numplayers, "numplayers");
	lua_setglobalcfunction(L, engageheight, "engageheight");
	lua_setglobalcfunction(L, engagedistance, "engagedistance");
	lua_setglobalcfunction(L, engagepos, "engageposition");
	lua_setglobalcfunction(L, engagerot, "engagerotation");
	lua_setglobalcfunction(L, engageplayerpos, "engageplayerpos");
	lua_setglobalcfunction(L, engageplayerrot, "engageplayerrot");
	lua_setglobalcfunction(L, friction, "friction");
	lua_setglobalcfunction(L, gravity, "gravity");
	lua_setglobalcfunction(L, api_object, "object");
	lua_setglobalcfunction(L, api_player, "player");
	lua_setglobalcfunction(L, api_body, "body");
	lua_setglobalcfunction(L, api_joint, "joint");
	lua_setglobalcfunction(L, shape, "shape");
	lua_setglobalcfunction(L, position, "position");
	lua_setglobalcfunction(L, orientation, "orientation");
	lua_setglobalcfunction(L, sides, "sides");
	lua_setglobalcfunction(L, density, "density");
	lua_setglobalcfunction(L, api_static_state, "static");
	lua_setglobalcfunction(L, radius, "radius");
	lua_setglobalcfunction(L, length, "length");
	lua_setglobalcfunction(L, strength, "strength");
	lua_setglobalcfunction(L, strength_alt, "strength_alt");
	lua_setglobalcfunction(L, velocity, "velocity");
	lua_setglobalcfunction(L, velocity_alt, "velocity_alt");
	lua_setglobalcfunction(L, axis, "axis");
	lua_setglobalcfunction(L, axis_alt, "axis_alt");
	lua_setglobalcfunction(L, axis, "axis");
	lua_setglobalcfunction(L, axis_alt, "axis_alt");
	lua_setglobalcfunction(L, range, "range");
	lua_setglobalcfunction(L, range_alt, "range_alt");
	lua_setglobalcfunction(L, connections, "connections");
	lua_setglobalcfunction(L, connectionType, "connectionType");

	lua_setglobalcfunction(L, api_dofile, "dofile");
	lua_setglobalcfunction(L, api_readfile, "readfile");
}

void GameSetup()
{
	dMass mass;

	game.gameframe = 0;
	game.freeze = true;
	game.freeze_time = 50;
	game.freeze_t = 0;
	game.unfreeze_time = 0;
	game.unfreeze_t = 0;

	game.space = dHashSpaceCreate(0);
  	game.contactgroup = dJointGroupCreate(0);
	game.floor = dCreatePlane(game.space, 0, 0, 1, 0);

	dGeomSetCategoryBits(game.floor, StaticObjectCategoryBits);
	dGeomSetCollideBits(game.floor, StaticObjectCollideBits);

  dWorldSetGravity(game.world, game.gravity[0], game.gravity[1], game.gravity[2]);

	for (auto& [object_name, o] : object) {
		o.create(mass);
		o.make_static();
	}

	for (auto& [player_name, p] : player) {
		p.create(mass);
	}


	if (game.engageheight) {
		for (auto& [player_name, p] : player) {
			for (auto& [body_name, b] : p.body) {
				b.position[2] = b.position[2] + game.engageheight;
				b.freeze.position[2] = b.freeze.position[2] + game.engageheight;
			}
		
			for (auto& [joint_name, j] : p.joint) {
				j.position[2] = j.position[2] + game.engageheight;
				j.freeze.position[2] = j.freeze.position[2] + game.engageheight;
			}
		}
	}

	if (game.engagedistance) {
		int i = 0;
		for (auto& [player_name, p] : player) {
			Matrix m = MatrixRotateZ(DEG2RAD * (360/game.numplayers) * i);
			Quaternion q = QuaternionFromMatrix(m);
			Quaternion iq = QuaternionInvert(q);
			for (auto& [body_name, b] : p.body) {
				Quaternion p = QuaternionMultiply(
						iq,
						(Quaternion){
						b.position[0],
						b.position[1] + game.engagedistance,
						b.position[2],
						0.00
				});
				p = QuaternionMultiply(p, q);
				
				Quaternion b_q = QuaternionMultiply(
						q,
						(Quaternion){
						b.orientation[1],
						b.orientation[2],
						b.orientation[3],
						b.orientation[0],
				});

				b.orientation[0] = b_q.w;
				b.freeze.orientation[0] = b.orientation[0];
				b.orientation[1] = b_q.x;
				b.freeze.orientation[1] = b.orientation[1];
				b.orientation[2] = b_q.y;
				b.freeze.orientation[2] = b.orientation[2];
				b.orientation[3] = b_q.z;
				b.freeze.orientation[3] = b.orientation[3];

				b.position[0] = p.x;
				b.freeze.position[0] = b.position[0];
				b.position[1] = p.y;
				b.freeze.position[1] = b.position[1];
			}

			for (auto& [joint_name, j] : p.joint) {
				Quaternion p = QuaternionMultiply(
						iq,
						(Quaternion){
							j.position[0],
							j.position[1] + game.engagedistance,
							j.position[2],
							0.00
				});
				p = QuaternionMultiply(p, q);

				Quaternion j_q = QuaternionMultiply(
						q,
						(Quaternion){
						j.orientation[1],
						j.orientation[2],
						j.orientation[3],
						j.orientation[0],
				});

				j.orientation[0] = j_q.w;
				j.freeze.orientation[0] = j.orientation[0];
				j.orientation[1] = j_q.x;
				j.freeze.orientation[1] = j.orientation[1];
				j.orientation[2] = j_q.y;
				j.freeze.orientation[2] = j.orientation[2];
				j.orientation[3] = j_q.z;
				j.freeze.orientation[3] = j.orientation[3];

				j.position[0] = p.x;
				j.freeze.position[0] = j.position[0];
				j.position[1] = p.y;
				j.freeze.position[1] = j.position[1];
			}
		i++;
		}
	}

}

void GameReset()
{
	dJointGroupDestroy(game.contactgroup);
	dSpaceDestroy(game.space);

	std::map<std::string, Object> new_object_map;
	object = new_object_map;

	std::map<std::string, Player> new_player_map;
	player = new_player_map;

	GameSetup();
}


void GameStart()
{
	dInitODE2(0);

	game.world = dWorldCreate();

	GameSetup();
}

void UpdateFreeze()
{
	game.freeze = true;
	game.unfreeze_t = 0;

	for (auto& [object_name, o] : object) {
		o.update_freeze();
	}

	for (auto& [player_name, p] : player) {
		p.update_freeze();
	}
}

void ReFreeze()
{
	game.freeze_t = 0;

	for (auto& [object_name, o] : object) {
		o.refreeze();
	}

	for (auto& [player_name, p] : player) {
		p.refreeze();
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

void ToggleGhosts()
{
	for (auto& [player_name, p] : player) {
		if (player_name != game.selected_player) {
			p.toggle_ghost();
		}
	}
}

void DrawFrame()
{
	DrawFloor();
	if (game.freeze) {
		for (auto& [object_name, o] : object) {
			o.draw_freeze();
			o.draw_ghost();
		}
				
		for (auto& [player_name, p] : player) {
			p.draw_freeze();
			p.draw_ghost();
		}
	} else {
		for (auto& [object_name, o] : object) {
			o.draw();
		}
		for (auto& [player_name, p] : player) {
			p.draw();
		}
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

void TriggerPlayerPassiveStates(std::string player_name, PlayerPassiveStates state)
{
	dReal strength = 0.00;
	for (auto& [joint_name, j] : player[player_name].joint) {
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

void TriggerPlayerPassiveStatesAlt(std::string player_name, PlayerPassiveStates state)
{
	dReal strength = 0.00;
	for (auto& [joint_name, j] : player[player_name].joint) {
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

void TogglePlayerPassiveStates(std::string player_name)
{
	ReFreeze();
	if (player[player_name].passive_states == RELAX_ALL) {
		TriggerPlayerPassiveStates(player_name, HOLD_ALL);
		player[player_name].passive_states = HOLD_ALL;
	} else {
		TriggerPlayerPassiveStates(player_name, RELAX_ALL);
		player[player_name].passive_states = RELAX_ALL;
	}
}

void TogglePlayerPassiveStatesAlt(std::string player_name)
{
	ReFreeze();
	if (player[player_name].passive_states_alt == RELAX_ALL) {
		TriggerPlayerPassiveStatesAlt(player_name, HOLD_ALL);
		player[player_name].passive_states_alt = HOLD_ALL;
	} else {
		TriggerPlayerPassiveStatesAlt(player_name, RELAX_ALL);
		player[player_name].passive_states_alt = RELAX_ALL;
	}
}

void RelaxAll(std::string player_name)
{
	ReFreeze();
	TriggerPlayerPassiveStates(player_name, RELAX_ALL);
	player[player_name].passive_states = RELAX_ALL;
}

void RelaxAllAlt(std::string player_name)
{
	ReFreeze();
	TriggerPlayerPassiveStatesAlt(player_name, RELAX_ALL);
	player[player_name].passive_states_alt = RELAX_ALL;
}

void TriggerActiveStateAlt(std::string player_name, std::string joint_name, dReal direction)
{
	switch(player[player_name].joint[joint_name].connectionType) {
		case Hinge: {
		} break;
		case Slider: {
		} break;
		case Universal: {
			dJointSetUniversalParam(player[player_name].joint[joint_name].dJoint[0], dParamFMax2,player[player_name].joint[joint_name].strength_alt);
			dJointSetUniversalParam(player[player_name].joint[joint_name].dJoint[0], dParamVel2, direction *player[player_name].joint[joint_name].velocity_alt);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(player[player_name].joint[joint_name].dJoint[0], dParamFMax2,player[player_name].joint[joint_name].strength_alt);
			dJointSetHinge2Param(player[player_name].joint[joint_name].dJoint[0], dParamVel2, direction *player[player_name].joint[joint_name].velocity_alt);
		} break;
	}
}

void TriggerPassiveStateAlt(std::string player_name, std::string joint_name, dReal strength)
{
	switch(player[player_name].joint[joint_name].connectionType) {
		case Hinge: {
		} break;
		case Slider: {
		} break;
		case Universal: {
			dJointSetUniversalParam(player[player_name].joint[joint_name].dJoint[0], dParamFMax2, strength);
			dJointSetUniversalParam(player[player_name].joint[joint_name].dJoint[0], dParamVel2, 0.0f);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(player[player_name].joint[joint_name].dJoint[0], dParamFMax2, strength);
			dJointSetHinge2Param(player[player_name].joint[joint_name].dJoint[0], dParamVel2, 0.0f);
		} break;
	}
}
void TriggerActiveState(std::string player_name, std::string joint_name, dReal direction)
{
	switch(player[player_name].joint[joint_name].connectionType) {
		case Hinge: {
			dJointSetHingeParam(player[player_name].joint[joint_name].dJoint[0], dParamFMax,player[player_name].joint[joint_name].strength);
			dJointSetHingeParam(player[player_name].joint[joint_name].dJoint[0], dParamVel, direction *player[player_name].joint[joint_name].velocity);
		} break;
		case Slider: {
			dJointSetSliderParam(player[player_name].joint[joint_name].dJoint[0], dParamFMax,player[player_name].joint[joint_name].strength);
			dJointSetSliderParam(player[player_name].joint[joint_name].dJoint[0], dParamVel, direction *player[player_name].joint[joint_name].velocity);
		} break;
		case Universal: {
			dJointSetUniversalParam(player[player_name].joint[joint_name].dJoint[0], dParamFMax,player[player_name].joint[joint_name].strength);
			dJointSetUniversalParam(player[player_name].joint[joint_name].dJoint[0], dParamVel, direction *player[player_name].joint[joint_name].velocity);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(player[player_name].joint[joint_name].dJoint[0], dParamFMax,player[player_name].joint[joint_name].strength);
			dJointSetHinge2Param(player[player_name].joint[joint_name].dJoint[0], dParamVel, direction *player[player_name].joint[joint_name].velocity);
		} break;
	}
}

void TriggerPassiveState(std::string player_name, std::string joint_name, dReal strength)
{
	switch(player[player_name].joint[joint_name].connectionType) {
		case Hinge: {
			dJointSetHingeParam(player[player_name].joint[joint_name].dJoint[0], dParamFMax, strength);
			dJointSetHingeParam(player[player_name].joint[joint_name].dJoint[0], dParamVel, 0.0f);
		} break;
		case Slider: {
			dJointSetSliderParam(player[player_name].joint[joint_name].dJoint[0], dParamFMax, strength);
			dJointSetSliderParam(player[player_name].joint[joint_name].dJoint[0], dParamVel, 0.0f);
		} break;
		case Universal: {
			dJointSetUniversalParam(player[player_name].joint[joint_name].dJoint[0], dParamFMax, strength);
			dJointSetUniversalParam(player[player_name].joint[joint_name].dJoint[0], dParamVel, 0.0f);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(player[player_name].joint[joint_name].dJoint[0], dParamFMax, strength);
			dJointSetHinge2Param(player[player_name].joint[joint_name].dJoint[0], dParamVel, 0.0f);
		} break;
	}
}

void TogglePassiveState(std::string player_name, std::string joint_name)
{
	if (joint_name == "NONE") return;

	ReFreeze();

	if (player[player_name].joint[joint_name].state == RELAX) {
		player[player_name].joint[joint_name].state = HOLD;
		TriggerPassiveState(player_name, joint_name, player[player_name].joint[joint_name].strength);
	} else {
		player[player_name].joint[joint_name].state = RELAX;
		TriggerPassiveState(player_name, joint_name, 0);
	}
}

void TogglePassiveStateAlt(std::string player_name, std::string joint_name)
{
	if (joint_name == "NONE") return;

	ReFreeze();

	if (player[player_name].joint[joint_name].state_alt == RELAX) {
		player[player_name].joint[joint_name].state_alt = HOLD;
		TriggerPassiveStateAlt(player_name, joint_name, player[player_name].joint[joint_name].strength);
	} else {
		player[player_name].joint[joint_name].state_alt = RELAX;
		TriggerPassiveStateAlt(player_name, joint_name, 0);
	}
}

void ToggleActiveState(std::string player_name, std::string joint_name)
{
	if (joint_name == "NONE") return;

	ReFreeze();
	
	if (player[player_name].joint[joint_name].state == FORWARD) {
		player[player_name].joint[joint_name].state = BACKWARD;
		TriggerActiveState(player_name, joint_name, -1);
	} else {
		player[player_name].joint[joint_name].state = FORWARD;
		TriggerActiveState(player_name, joint_name, 1);
	}
}

void ToggleActiveStateAlt(std::string player_name, std::string joint_name)
{
	if (joint_name == "NONE") return;

	ReFreeze();
	
	if (player[player_name].joint[joint_name].state_alt == FORWARD) {
		player[player_name].joint[joint_name].state_alt = BACKWARD;
		TriggerActiveStateAlt(player_name, joint_name, -1);
	} else {
		player[player_name].joint[joint_name].state_alt = FORWARD;
		TriggerActiveStateAlt(player_name, joint_name, 1);
	}
}


void CycleState(std::string player_name, std::string joint_name)
{
	if (joint_name == "NONE") return;

	ReFreeze();
	
	if (player[player_name].joint[joint_name].state == FORWARD) {
		player[player_name].joint[joint_name].state = BACKWARD;
		TriggerActiveState(player_name, joint_name, -1);
	} else if (player[player_name].joint[joint_name].state == BACKWARD) {
		player[player_name].joint[joint_name].state = HOLD;
		TriggerPassiveState(player_name, joint_name, player[player_name].joint[joint_name].strength);
	} else if (player[player_name].joint[joint_name].state == HOLD) {
		player[player_name].joint[joint_name].state = RELAX;
		TriggerPassiveState(player_name, joint_name, 0);
	} else {
		player[player_name].joint[joint_name].state = FORWARD;
		TriggerActiveState(player_name, joint_name, 1);
	}
}


void CycleStateAlt(std::string player_name, std::string joint_name)
{
	if (joint_name == "NONE") return;

	ReFreeze();
	
	if (player[player_name].joint[joint_name].state_alt == FORWARD) {
		player[player_name].joint[joint_name].state_alt = BACKWARD;
		TriggerActiveStateAlt(player_name, joint_name, -1);
	} else if (player[player_name].joint[joint_name].state_alt == BACKWARD) {
		player[player_name].joint[joint_name].state_alt = HOLD;
		TriggerPassiveStateAlt(player_name, joint_name, player[player_name].joint[joint_name].strength_alt);
	} else if (player[player_name].joint[joint_name].state_alt == HOLD) {
		player[player_name].joint[joint_name].state_alt = RELAX;
		TriggerPassiveStateAlt(player_name, joint_name, 0);
	} else {
		player[player_name].joint[joint_name].state_alt = FORWARD;
		TriggerActiveStateAlt(player_name, joint_name, 1);
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

		o.freeze.linear_vel[0] = 0.00;
		o.freeze.linear_vel[1] = 0.00;
		o.freeze.linear_vel[2] = 0.00;

		o.freeze.angular_vel[0] = 0.00;
		o.freeze.angular_vel[1] = 0.00;
		o.freeze.angular_vel[2] = 0.00;
	}
	
	for (auto& [player_name, p] : player) {
		for (auto& [body_name, b] : p.body) {
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
	
			b.freeze.linear_vel[0] = 0.00;
			b.freeze.linear_vel[1] = 0.00;
			b.freeze.linear_vel[2] = 0.00;
	
			b.freeze.angular_vel[0] = 0.00;
			b.freeze.angular_vel[1] = 0.00;
			b.freeze.angular_vel[2] = 0.00;
		}
		
		for (auto& [joint_name, j] : p.joint) {
			dGeomSetPosition(
				j.dGeom,
				j.position[0],
				j.position[1],
				j.position[2]);
			dGeomSetQuaternion(j.dGeom, j.orientation);
			dBodySetLinearVel(j.dBody, 0.00, 0.00, 0.00);
			dBodySetAngularVel(j.dBody, 0.00, 0.00, 0.00);
	
			j.freeze.position[0] = j.position[0];
			j.freeze.position[1] = j.position[1];
			j.freeze.position[2] = j.position[2];
	
			j.freeze.orientation[0] = j.orientation[0];
			j.freeze.orientation[1] = j.orientation[1];
			j.freeze.orientation[2] = j.orientation[2];
			j.freeze.orientation[3] = j.orientation[3];
	
			j.freeze.linear_vel[0] = 0.00;
			j.freeze.linear_vel[1] = 0.00;
			j.freeze.linear_vel[2] = 0.00;
	
			j.freeze.angular_vel[0] = 0.00;
			j.freeze.angular_vel[1] = 0.00;
			j.freeze.angular_vel[2] = 0.00;
		}
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
	int size = player[game.selected_player].body.size(); 
	if (size > 0) {
		float x = 0.00;
		float y = 0.00;
		float z = 0.00;
	
		for (auto const [body_name, b] : player[game.selected_player].body) {
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
	} else {
		camera->target = (Vector3){ 0.00, 0.00, 0.00 };
		camera->position = (Vector3){
			CameraOffset->x,
			CameraOffset->y,
			CameraOffset->z
		};
	}
}

void SelectPlayer (Camera3D Camera, Ray MouseRay, RayCollision MouseCollision)
{
	RayCollision collision = { 0 };
	MouseRay = GetMouseRay(GetMousePosition(), Camera);
	for (auto& [player_name, p] : player) {
		for (auto& [body_name, b] : p.body) {
			MouseCollision = b.collide_mouse_ray(MouseRay, MouseCollision);
			if (MouseCollision.hit) {
				collision = MouseCollision;
				game.selected_player = player_name;
				break;
			}
		}
		for (auto& [joint_name, j] : p.joint) {
			MouseCollision = j.collide_mouse_ray(MouseRay, MouseCollision);
			if (MouseCollision.hit) {
				collision = MouseCollision;
				game.selected_player = player_name;
				break;
			}
		}
	}
}

void SelectJoint (Camera3D Camera, Ray MouseRay, RayCollision MouseCollision)
{
	RayCollision collision = { 0 };
	MouseRay = GetMouseRay(GetMousePosition(), Camera);
	for (auto& [joint_name, j] : player[game.selected_player].joint) {
		MouseCollision = j.collide_mouse_ray(MouseRay, MouseCollision);
		if (MouseCollision.hit) {
			collision = MouseCollision;
			game.selected_joint = j.name;
			j.select = true;
			break;
		} else {
			game.selected_joint = "NONE";
			j.select = false;
		}
	}
}

std::map<int, FrameData> RecordedFrames;

void RecordFrame(int gameframe)
{
	std::ofstream tempframefile("tempframefile.txt");
	tempframefile << "FRAME " << gameframe << "\n";
	for (auto const& [player_name, p] : player) {
		RecordedFrames[gameframe].player[player_name] = p;
		for (auto const& [joint_name, j] : p.joint) {
			tempframefile << "JOINT " <<
				player_name << " " <<
				joint_name << " " <<
				j.state << " " <<
				j.state_alt << std::endl;
		}
		for (auto const& [body_name, b] : p.body) {
			tempframefile << "POS " <<
				player_name << " " <<
				body_name << " " <<
				b.freeze.position[0] << " " <<
				b.freeze.position[1] << " " <<
				b.freeze.position[2] << std::endl;
			tempframefile << "QAT " <<
				player_name << " " <<
				body_name << " " <<
				b.freeze.orientation[0] << " " <<
				b.freeze.orientation[1] << " " <<
				b.freeze.orientation[2] << " " <<
				b.freeze.orientation[3] << std::endl;
			tempframefile << "LINVEL " <<
				player_name << " " <<
				body_name << " " <<
				b.freeze.linear_vel[0] << " " <<
				b.freeze.linear_vel[1] << " " <<
				b.freeze.linear_vel[2] << std::endl;
			tempframefile << "ANGVEL " <<
				player_name << " " <<
				body_name << " " <<
				b.freeze.angular_vel[0] << " " <<
				b.freeze.angular_vel[1] << " " <<
				b.freeze.angular_vel[2] << std::endl;
		}
	}
	tempframefile.close();

	std::ofstream tempreplayfile("tempreplayfile.txt", std::ios::app);
	tempreplayfile << "FRAME " << gameframe << "\n";
	for (auto const& [player_name, p] : player) {
		tempreplayfile << "PLAYER " << player_name << "\n";
		RecordedFrames[gameframe].player[player_name] = p;
		for (auto const& [joint_name, j] : p.joint) {
			tempreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
		}
	}
	tempreplayfile.close();
}

void PlayFrame(int gameframe)
{
	FrameData frame = RecordedFrames[gameframe];
	for (auto& [player_name, p] : frame.player) {
		for (auto& [joint_name, j] : p.joint) {
			switch(j.state) {
				case RELAX: {
					j.state = RELAX;
					TriggerPassiveState(p.name, j.name, 0);
				} break;
				case HOLD: {
					j.state = HOLD;
					TriggerPassiveState(p.name, j.name, j.strength);
				} break;
				case FORWARD: {
					j.state = FORWARD;
					TriggerActiveState(p.name, j.name, 1);
				} break; 
				case BACKWARD: {
					j.state = BACKWARD;
					TriggerActiveState(p.name, j.name, -1);
				} break;
			}
		
			switch(j.state_alt) {
				case RELAX: {
					j.state_alt = RELAX;
					TriggerPassiveStateAlt(p.name, j.name, 0);
				} break;
				case HOLD: {
					j.state_alt = HOLD;
					TriggerPassiveStateAlt(p.name, j.name, j.strength_alt);
				} break;
				case FORWARD: {
					j.state_alt = FORWARD;
					TriggerActiveStateAlt(p.name, j.name, 1);
				} break;
				case BACKWARD: {
					j.state_alt = BACKWARD;
					TriggerActiveStateAlt(p.name, j.name, -1);
				} break;
			}
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
	for (auto const& [player_name, p] : player) {
		tempframefile << "PLAYER " << player_name << "\n";
		for (auto const& [joint_name, j] : p.joint) {
			tempframefile << j.name << " " << j.state << " " << j.state_alt << "\n";
		}
	}
	tempframefile.close();

	std::ofstream tempreplayfile("tempreplayfile.txt");
	for (auto const& [gameframe, frame] : RecordedFrames) {
		if (gameframe > game.gameframe) {
			break;
		} else {
			tempreplayfile << "FRAME " << gameframe << "\n";
			for (auto const& [player_name, p] : frame.player) {
				tempreplayfile << "FRAME " << player_name << "\n";
				for (auto const& [joint_name, j] : p.joint) {
					tempreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
				}
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
		for (auto const& [player_name, p] : frame.player) {
			savedreplayfile << "PLAYER " << player_name << "\n";
			for (auto const& [joint_name, j] : p.joint) {
				savedreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
			}
		}
	}
	savedreplayfile.close();
}

void StartFreeplay()
{
	game.gamemode = FREEPLAY;

	for (auto const& [player_name, p] : player) {
		RelaxAll(player_name);
		RelaxAllAlt(player_name);
	}

	game.freeze = true;
	game.gameframe = 0;
	game.freeze_t = 0;
	game.unfreeze_t = 0;
	
	Restart();

	UpdateFreeze();

	std::ofstream tempreplayfile("tempreplayfile.txt");
	tempreplayfile << "FRAME 0\n";
	for (auto const& [player_name, p] : player) {
		tempreplayfile << "PLAYER " << player_name << "\n";
		for (auto const& [joint_name, j] : p.joint) {
			RecordedFrames[0].player[player_name].joint[joint_name] = j;
			tempreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
		}
	}
	tempreplayfile.close();
}

void StartReplay()
{
	game.gamemode = REPLAY;

	for (auto const& [player_name, p] : player) {
		RelaxAll(player_name);
		RelaxAllAlt(player_name);
	}

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
	lua_State* L = luaL_newstate();
	api_init(L);
	api_state_getters(L);
	api_draw_functions(L);
	luaL_openlibs(L);

	if (luau_dofile(L, "scripts/init.luau") != LUA_OK) {
		luaL_error(L, "Error: %s\n", lua_tostring(L, -1));
	}

	//luaL_sandbox(L);

	SetTraceLogLevel(LOG_ERROR);
	InitWindow(1600, 900, "TOBAS");
	SetExitKey(0);

	Camera3D camera = { 0 };
	camera.up = (Vector3){ 0.00, 0.00, 1.00 };
	camera.fovy = 45.00;
	camera.projection = CAMERA_PERSPECTIVE;

	Vector3 CameraOffset = (Vector3){ 0.00, -5.00, 0.00 };
	Vector3 CameraZoom = (Vector3){ 0.00, 0.00, 0.00 };

	SetTargetFPS(60);
	
	GameStart();

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
					ToggleActiveStateAlt(game.selected_player, game.selected_joint);
				} else {
					ToggleActiveState(game.selected_player, game.selected_joint);
				}
			}
		}

		if (IsKeyPressed(KEY_X)) {
			if (IsKeyDown(KEY_LEFT_SHIFT)) {
				TogglePassiveStateAlt(game.selected_player, game.selected_joint);
			} else {
				TogglePassiveState(game.selected_player, game.selected_joint);
			}
		}

		if (IsKeyPressed(KEY_C)) {
			TogglePlayerPassiveStates(game.selected_player);
			TogglePlayerPassiveStatesAlt(game.selected_player);
		}

		if (IsMouseButtonPressed(0)) {
			SelectPlayer(camera, MouseRay, MouseCollision);
			if (IsKeyDown(KEY_LEFT_SHIFT)) {
				CycleStateAlt(game.selected_player, game.selected_joint);
			} else {
				CycleState(game.selected_player, game.selected_joint);
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

		if (IsKeyPressed(KEY_G)) {
			ToggleGhosts();
		}

		if (IsKeyPressed(KEY_ESCAPE)) {
			GameReset();
		}

		UpdateFrame();
	
		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
				DrawFrame();
			EndMode3D();
			api_draw2d(L);
		EndDrawing();
	}
	
	GameEnd();
	CloseWindow();
	lua_close(L);

	return 0;
}
