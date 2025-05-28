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
	std::string selected_player = "NONE";
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
	Color color;
	Color ghost_color;

	unsigned long category_bits;
	unsigned long collide_bits;

	Object() {
		ghost_color = (Color){ 51, 51, 51, 51 };
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

		if (static_state) {
			dJointID fixed = dJointCreateFixed(game.world ,0);
			dJointAttach(fixed, dBody, 0);
			dJointSetFixed(fixed);
		}

		dGeomSetCategoryBits(dGeom, category_bits);
		dGeomSetCollideBits(dGeom, collide_bits);
	};

	void update_freeze() {
		const dReal *position = dBodyGetPosition(dBody);
		const dReal *orientation = dBodyGetQuaternion(dBody);
		const dReal *linearVel = dBodyGetLinearVel(dBody);
		const dReal *angularVel = dBodyGetAngularVel(dBody);

		freeze.position[0] = position[0];
		freeze.position[1] = position[1];
		freeze.position[2] = position[2];

		freeze.orientation[0] = orientation[0];
		freeze.orientation[1] = orientation[1];
		freeze.orientation[2] = orientation[2];
		freeze.orientation[3] = orientation[3];

		freeze.linearVel[0] = linearVel[0];
		freeze.linearVel[1] = linearVel[1];
		freeze.linearVel[2] = linearVel[2];

		freeze.angularVel[0] = angularVel[0];
		freeze.angularVel[1] = angularVel[1];
		freeze.angularVel[2] = angularVel[2];
	};

	void refreeze() {
		dBodySetPosition(
			dBody,
			freeze.position[0],
			freeze.position[1],
			freeze.position[2]);
		dBodySetQuaternion(dBody, freeze.orientation);
		dBodySetLinearVel(
			dBody,
			freeze.linearVel[0],
			freeze.linearVel[1],
			freeze.linearVel[2]);
		dBodySetAngularVel(
			dBody,
			freeze.angularVel[0],
			freeze.angularVel[1],
			freeze.angularVel[2]);
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
		dRtoQ(dBodyGetRotation(dBody), dQ);

		float angle;
		Vector3 axis;
		Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
		QuaternionToAxisAngle(q, &axis, &angle);
		const dReal *pos = dBodyGetPosition(dBody);
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

		draw_object(color);
	};

	void draw_ghost() {
		draw(ghost_color);
	};

	void draw() {
		draw(color);
	};

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
		
				dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(dJoint[1], dBody, b2);
				dJointSetFixed(dJoint[1]);
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
		
				dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(dJoint[1], dBody, b2);
				dJointSetFixed(dJoint[1]);
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
		
				dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(dJoint[1], dBody, b2);
				dJointSetFixed(dJoint[1]);
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
		
				dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(dJoint[1], dBody, b2);
				dJointSetFixed(dJoint[1]);
			} break;
			default:
				dJoint[0] = dJointCreateFixed(game.world, 0);
				dJointAttach(dJoint[0], b1, dBody);
				dJointSetFixed(dJoint[0]);
		
				dJoint[1] = dJointCreateFixed(game.world, 0);
				dJointAttach(dJoint[1], dBody, b2);
				dJointSetFixed(dJoint[1]);
		}
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
	
	unsigned long body_category_bits;
	unsigned long body_collide_bits;
	unsigned long joint_category_bits;
	unsigned long joint_collide_bits;

	Color joint_color;
	Color body_color;
	Color ghost_color;

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
		for (auto& [body_name, b] : body) {
			b.draw_ghost();
		}
	
		for (auto& [joint_name, j] : joint) {
			j.draw_ghost();
		}
	};
};

class FrameData : public FreezeData {
	public:
	std::map<std::string, Player> player;
};

std::map<std::string, Object> object;
std::map<std::string, Player> player;

std::string MSG;

Color DynamicObjectColor = (Color){ 0, 255, 0, 255 };
Color StaticObjectColor = (Color){ 51, 51, 51, 255 };

std::string object_key;
std::string player_key;
std::string body_key;
std::string joint_key;

unsigned long StaticObjectCategoryBits = 0b0001;
unsigned long StaticObjectCollideBits = 0b0000;

unsigned long DynamicObjectCategoryBits = 0b0010;
unsigned long DynamicObjectCollideBits = 0b0001;

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

	object[object_key].freeze.linearVel[0] = 0.00;
	object[object_key].freeze.linearVel[1] = 0.00;
	object[object_key].freeze.linearVel[2] = 0.00;

	object[object_key].freeze.angularVel[0] = 0.00;
	object[object_key].freeze.angularVel[1] = 0.00;
	object[object_key].freeze.angularVel[2] = 0.00;

	object[object_key].category_bits = DynamicObjectCategoryBits;
	object[object_key].collide_bits = DynamicObjectCollideBits;

	object[object_key].static_state = false;

	lua_Number result = 1;

	lua_pushnumber(L, result);

	return 1;
}

int api_player(lua_State* L)
{
	DataContext = PlayerContext;

	std::string name = lua_tostring(L, -1);
	
	player_key = name;
	
	player[player_key].name = name;

	switch (player.size()) {
		case 1: {
			game.selected_player = player_key;
			player[player_key].joint_color = MAROON;
		} break;
		case 2: {
			player[player_key].joint_color = DARKBLUE;
		} break;
		case 3: {
			player[player_key].joint_color = DARKGREEN;
		} break;
		case 4: {
			player[player_key].joint_color = DARKPURPLE;
		} break;
	}

	player[player_key].ghost_color.a = 55;

	player[player_key].body_category_bits = BodyCategoryBits;
	player[player_key].body_collide_bits = BodyCollideBits;
	player[player_key].joint_category_bits = JointCategoryBits;
	player[player_key].joint_collide_bits = JointCollideBits;

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

	player[player_key].body[body_key].freeze.linearVel[0] = 0.00;
	player[player_key].body[body_key].freeze.linearVel[1] = 0.00;
	player[player_key].body[body_key].freeze.linearVel[2] = 0.00;

	player[player_key].body[body_key].freeze.angularVel[0] = 0.00;
	player[player_key].body[body_key].freeze.angularVel[1] = 0.00;
	player[player_key].body[body_key].freeze.angularVel[2] = 0.00;

	player[player_key].body[body_key].category_bits = player[player_key].body_category_bits;
	player[player_key].body[body_key].collide_bits = player[player_key].body_collide_bits;

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
	player[player_key].joint[joint_key].color = player[player_key].joint_color;
	player[player_key].joint[joint_key].ghost_color = player[player_key].ghost_color;

	player[player_key].joint[joint_key].orientation[0] = 1.00;
	player[player_key].joint[joint_key].orientation[1] = 0.00;
	player[player_key].joint[joint_key].orientation[2] = 0.00;
	player[player_key].joint[joint_key].orientation[3] = 0.00;

	player[player_key].joint[joint_key].freeze.orientation[0] = 1.00;
	player[player_key].joint[joint_key].freeze.orientation[1] = 0.00;
	player[player_key].joint[joint_key].freeze.orientation[2] = 0.00;
	player[player_key].joint[joint_key].freeze.orientation[3] = 0.00;

	player[player_key].joint[joint_key].freeze.linearVel[0] = 0.00;
	player[player_key].joint[joint_key].freeze.linearVel[1] = 0.00;
	player[player_key].joint[joint_key].freeze.linearVel[2] = 0.00;

	player[player_key].joint[joint_key].freeze.angularVel[0] = 0.00;
	player[player_key].joint[joint_key].freeze.angularVel[1] = 0.00;
	player[player_key].joint[joint_key].freeze.angularVel[2] = 0.00;

	player[player_key].joint[joint_key].category_bits = player[player_key].joint_category_bits;
	player[player_key].joint[joint_key].collide_bits = player[player_key].joint_collide_bits;

	player[player_key].joint[joint_key].state = RELAX;
	player[player_key].joint[joint_key].state_alt = RELAX;

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
			if (game.engageplayerpos) {
				player[player_key].body[body_key].position[0] = position[0] + game.engageplayerpos[0];
				player[player_key].body[body_key].position[1] = position[1] + game.engageplayerpos[1];
				player[player_key].body[body_key].position[2] = position[2] + game.engageplayerpos[2];
				player[player_key].body[body_key].freeze.position[0] = position[0] + game.engageplayerpos[0];
				player[player_key].body[body_key].freeze.position[1] = position[1] + game.engageplayerpos[1];
				player[player_key].body[body_key].freeze.position[2] = position[2] + game.engageplayerpos[2];

			} else {
				player[player_key].body[body_key].position[0] = position[0];
				player[player_key].body[body_key].position[1] = position[1];
				player[player_key].body[body_key].position[2] = position[2];
				player[player_key].body[body_key].freeze.position[0] = position[0];
				player[player_key].body[body_key].freeze.position[1] = position[1];
				player[player_key].body[body_key].freeze.position[2] = position[2];
			}
		} break;
		case JointContext: {
			if (game.engageplayerpos) {
				player[player_key].joint[joint_key].position[0] = position[0] + game.engageplayerpos[0];
				player[player_key].joint[joint_key].position[1] = position[1] + game.engageplayerpos[1];
				player[player_key].joint[joint_key].position[2] = position[2] + game.engageplayerpos[2];
				player[player_key].joint[joint_key].freeze.position[0] = position[0] + game.engageplayerpos[0];
				player[player_key].joint[joint_key].freeze.position[1] = position[1] + game.engageplayerpos[1];
				player[player_key].joint[joint_key].freeze.position[2] = position[2] + game.engageplayerpos[2];
			} else {
				player[player_key].joint[joint_key].position[0] = position[0];
				player[player_key].joint[joint_key].position[1] = position[1];
				player[player_key].joint[joint_key].position[2] = position[2];
				player[player_key].joint[joint_key].freeze.position[0] = position[0];
				player[player_key].joint[joint_key].freeze.position[1] = position[1];
				player[player_key].joint[joint_key].freeze.position[2] = position[2];
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

int composite(lua_State* L)
{
	std::string name = lua_tostring(L, -1);
	
	switch(DataContext) {
		case NoContext: {
			// Error Handling
		} break;
		case ObjectContext: {
			object[object_key].composite.name = name;
		} break;
		case BodyContext: {
			player[player_key].body[body_key].composite.name = name;
		} break;
		case JointContext: {
			player[player_key].joint[joint_key].composite.name = name;
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

	lua_pushcfunction(L, api_player);
	lua_setglobal(L, "player");

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

	dGeomSetCategoryBits(game.floor, StaticObjectCategoryBits);
	dGeomSetCollideBits(game.floor, StaticObjectCollideBits);

  	dWorldSetGravity(game.world, game.gravity[0], game.gravity[1], game.gravity[2]);
	
	for (auto& [object_name, o] : object) {
		o.create(mass);
	}

	for (auto& [player_name, p] : player) {
		p.create(mass);
	}
}

void UpdateFreeze()
{
	game.freeze = true;
	game.unfreeze_t = 0;

	for (auto& [obejct_name, o] : object) {
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

		o.freeze.linearVel[0] = 0.00;
		o.freeze.linearVel[1] = 0.00;
		o.freeze.linearVel[2] = 0.00;

		o.freeze.angularVel[0] = 0.00;
		o.freeze.angularVel[1] = 0.00;
		o.freeze.angularVel[2] = 0.00;
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
	
			b.freeze.linearVel[0] = 0.00;
			b.freeze.linearVel[1] = 0.00;
			b.freeze.linearVel[2] = 0.00;
	
			b.freeze.angularVel[0] = 0.00;
			b.freeze.angularVel[1] = 0.00;
			b.freeze.angularVel[2] = 0.00;
		}
		
		for (auto& [joint_name, j] : p.joint) {
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

void SelectPlayer ()
{

}

void SelectJoint (Camera3D Camera, Ray MouseRay, RayCollision MouseCollision)
{
	MouseRay = GetMouseRay(GetMousePosition(), Camera);
	for (auto const& [joint_name, j] : player[game.selected_player].joint) {
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
	for (auto const& [player_name, p] : player) {
		tempframefile << "PLAYER " << player_name << "\n";
		RecordedFrames[gameframe].player[player_name] = p;
		for (auto const& [joint_name, j] : p.joint) {
			tempframefile << j.name << " " << j.state << " " << j.state_alt << "\n";
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
	SetTraceLogLevel(LOG_ERROR);

	InitWindow(1600, 900, "TOBAS");

	Camera3D camera = { 0 };
	camera.up = (Vector3){ 0.00, 0.00, 1.00 };
	camera.fovy = 45.00;
	camera.projection = CAMERA_PERSPECTIVE;

	Vector3 CameraOffset = (Vector3){ 0.00, -5.00, 0.00 };
	Vector3 CameraZoom = (Vector3){ 0.00, 0.00, 0.00 };

	SetTargetFPS(60);
	
	GameStart();

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
	tempreplayfile << "FRAME " << game.gameframe << "\n";
	for (auto const& [player_name, p] : player) {
		tempreplayfile << "PLAYER " << player_name << "\n";
		for (auto const& [joint_name, j] : p.joint) {
			tempreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
		}
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
