#ifndef BODY_H
#define BODY_H

enum Shape {
	Box,
	Sphere,
	Capsule,
	Cylinder,
	Composite,
};

class FreezeData {
	public:
	dReal position[3];
	dReal orientation[4];
	dReal angular_vel[3];
	dReal linear_vel[3];
};

class Body {
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

	dWorldID world;
	dSpaceID space;
	dMass mass;

	unsigned long category_bits;
	unsigned long collide_bits;

	Body();
	
	void make_static();
	
	void create();
	
	void update_freeze();
	
	void refreeze();
	
	void draw_object(Color draw_color);
	
	void draw(Color draw_color);
	
	void draw_freeze();
	
	void draw_ghost();
	
	void draw();
	
	void toggle_ghost();
	
	RayCollision collide_mouse_ray(Ray ray, RayCollision collision);
};
#endif
