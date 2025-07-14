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

	Vector3 freeze_linear_vel;
	Vector3 freeze_angulrt_vel;

	Vector3 freeze_position;
	Vector3 freeze_orientation;

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

	uint32_t cat_bits;
	uint32_t col_bits;

	Body() {};
	Body(std::string_view name);

	void create_geom(dWorldID world, dSpaceID space);
	void create_body(dWorldID world, dSpaceID space);
	void create_static(dWorldID world, dSpaceID space);
	void create_dynamic(dWorldID world, dSpaceID space);

	void set_cat_bits(uint32_t bits);
	void set_cat_bits();
	void set_col_bits(uint32_t bits);
	void set_col_bits();

	void make_static(dWorldID world);

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
