#pragma once
#include "common.h"

typedef int BodyID;

enum BodyShape {
	Box,
	Sphere,
	Capsule,
	Cylinder,
	Composite,
};

struct FreezeData {
	Vector3 position;
	Vector4 orientation;

	Vector3 linear_vel;
	Vector3 angular_vel;
};

struct Body {
	std::string m_name;

	dMass mass;

	dBodyID dBody;
	dGeomID dGeom;

	BodyShape shape;

	Vector3 position;
	Vector3 offset;
	Vector4 orientation;
	Vector3 sides;

	dReal radius;
	dReal length;
	dReal density;

	FreezeData freeze;

	Color color;
	Color ghost_color;
	Color select_color;
	//for grips
	bool active;

	bool select;
	bool ghost;
	bool static_state;

	unsigned long category_bits;
	unsigned long collide_bits;

	Body();
	Body(std::string_view name);

	void make_static(dWorldID world);
	void set_category_bits(unsigned long bits);
	void set_category_bits();
	void set_collide_bits(unsigned long bits);
	void set_collide_bits();
	void create(dWorldID world, dSpaceID space);
	void update_freeze();
	void refreeze();
	void reset();
	void draw_object(Color draw_color);
	void draw(Color draw_color);
	void draw_freeze();
	void draw_ghost();
	void draw(bool freeze);
	void toggle_ghost();
	std::string get_name();
	RayCollision collide_mouse_ray(Ray ray, RayCollision collision);
};

struct env_joint
{
	std::string_view name;
	BodyID connections[2];
	//JointType type;
	Vector3 position;
	Vector3 axis;
	dReal range[2];
	dReal strength;
	dReal velocity;
};
