#pragma once
#include "common.h"

typedef int BodyID;

enum BodyShape
{
	BOX,
	SPHERE,
	CAPSULE,
	CYLINDER,
	COMPOSITE,
};

struct BodyUserData
{
	bool active;
	dContact contact;
	dJointID contact_joint = nullptr;
};

struct Body
{
	BodyUserData m_data;

	BodyID m_id;

	std::string m_name;

	dMass mass;

	dWorldID m_world;
	dSpaceID m_space;

	dBodyID dBody;
	dGeomID dGeom;

	BodyShape shape;

	dReal radius;
	dReal length;
	dReal density;


	raylib::Vector4 m_orientation;
	raylib::Vector3 m_position;
	raylib::Vector3 m_offset;
	raylib::Vector3 m_sides;

	raylib::Vector3 frame_linear_vel;
	raylib::Vector3 frame_angular_vel;

	raylib::Vector4 frame_orientation;
	raylib::Vector3 frame_position;
	raylib::Vector3 frame_offset;

	raylib::Vector3 freeze_linear_vel;
	raylib::Vector3 freeze_angular_vel;

	raylib::Vector4 freeze_orientation;
	raylib::Vector3 freeze_position;
	raylib::Vector3 freeze_offset;

	raylib::Color m_color;
	raylib::Color m_g_color;

	raylib::Color m_select_color;
	raylib::Color m_active_color;

	bool active;
	bool select;

	bool ghost;

	bool m_composite;

	bool m_static;
	bool m_interactive;
	uint32_t m_cat_bits;
	uint32_t m_col_bits;

	Body() {};
	Body(BodyID id, const char* name);

	void SetOffset(raylib::Vector3 offset);
	void SetColor(raylib::Color color);

	void Create(dWorldID world, dSpaceID space);

	void CreateGeom();
	void CreateBody();
	void CreateStatic();
	void CreateDynamic();
	void CreateComposite(dBodyID b);

	void Step();

	void SetCatBits(uint32_t bits);
	void SetCatBits();

	void SetColBits(uint32_t bits);
	void SetColBits();

	void Freeze();

	void Refreeze();
	void Reset();

	void DrawObject(raylib::Color color);
	void DrawObjectWires(raylib::Color color);

	void Draw(raylib::Color color);
	void Draw(bool freeze);
	void DrawFreeze();
	void DrawGhost();

	void ToggleGhost();

	BodyID GetID();
	std::string GetName();

	raylib::RayCollision collide_mouse_ray(raylib::Ray ray, raylib::RayCollision collision);
};

typedef int JointID;

enum JointType
{
	BALL = 1,
	HINGE,
	dSLIDER,

	UNIVERSAL,
	HINGE2,

	FIXED,
	CONTACT,
};

enum JointState
{
	RELAX,
	HOLD,
	FORWARD,
	BACKWARD,
};

struct Joint : public Body
{
	dJointID dJoint;

	JointType type;
	JointState state;
	JointState state_alt;

	BodyID connections[2];

	raylib::Vector3 axis;
	raylib::Vector3 axis_alt;

	dReal range[2];
	dReal range_alt[2];
	dReal strength;
	dReal strength_alt;
	dReal velocity;
	dReal velocity_alt;

	Joint(JointID id, const char* name);

	void Create(dWorldID world, dSpaceID space, Body b1, Body b2);

	void Draw(raylib::Color color);
	void Draw(bool freeze);
	void DrawAxis(bool freeze);
	void DrawRange(bool freeze);

	void TriggerActiveStateAlt(dReal direction);	
	void TriggerPassiveStateAlt(dReal strength);	
	void TriggerActiveState(dReal direction);	
	void TriggerPassiveState(dReal strength);	
	void TogglePassiveState();	
	void TogglePassiveStateAlt();	
	void ToggleActiveState();	
	void ToggleActiveStateAlt();	
	void CycleState();	
	void CycleStateAlt();
};
