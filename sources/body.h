#pragma once
#include <common.h>

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
	Vector4 orientation;
	Vector3 sides;

	dReal radius;
	dReal length;
	dReal density;

	FreezeData freeze;

	Color color;
	Color ghost_color;
	Color select_color;

	bool select;
	bool ghost;
	bool static_state;

	unsigned long category_bits;
	unsigned long collide_bits;

	Body();
	Body(const char* name);

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
	void draw();
	void toggle_ghost();
	std::string get_name();
	RayCollision collide_mouse_ray(Ray ray, RayCollision collision);
};
