#include <common.h>

enum BodyShape {
	Box,
	Sphere,
	Capsule,
	Cylinder,
	Composite,
};

class FreezeData {
	public:
	Vector3 position;
	Vector4 orientation;

	Vector3 linear_vel;
	Vector3 angular_vel;
};

class Body {
	public:
	dMass mass;
	//dWorldID world;
	//dSpaceID space;
	dBodyID dBody;
	dGeomID dGeom;

	std::string name;
	BodyShape shape;

	Vector3 position;
	Vector4 orientation;
	
	Vector3 sides;
	dReal radius;
	dReal length;
	dReal density;
	FreezeData freeze;
	bool static_state;
	Color color;
	Color ghost_color;
	Color select_color;
	bool select;
	bool ghost;

	unsigned long category_bits;
	unsigned long collide_bits;

	Body();
	
	void make_static(dWorldID world);
	void create(dWorldID world, dSpaceID space);
	void update_freeze();
	void ReFreeze();
	void draw_object(Color draw_color);
	void draw(Color draw_color);
	void draw_freeze();
	void draw_ghost();
	void draw();
	void toggle_ghost();
	RayCollision collide_mouse_ray(Ray ray, RayCollision collision);
};
