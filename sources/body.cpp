#include "body.h"
#include "common.h"
#include "ode/ode.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

Body::Body()
{
	color = GREEN;
	ghost_color = (Color){51, 51, 51, 51};

	position = (Vector3){0.00, 0.00, 0.00};

	orientation = (Vector4){0.00, 0.00, 0.00, 1.00};

	freeze.position = (Vector3){0.00, 0.00, 0.00};

	freeze.orientation = orientation;
	freeze.linear_vel = (Vector3){0.00, 0.00, 0.00};
	freeze.angular_vel = (Vector3){0.00, 0.00, 0.00};

	category_bits = 0b0001;
	collide_bits = 0b0000;

	select = false;
	static_state = false;
};

Body::Body(std::string_view name)
{
	m_name = name;
	color = GREEN;
	ghost_color = (Color){51, 51, 51, 51};

	position = (Vector3){0.00, 0.00, 0.00};

	orientation = (Vector4){0.00, 0.00, 0.00, 1.00};

	freeze.position = (Vector3){0.00, 0.00, 0.00};

	freeze.orientation = orientation;
	freeze.linear_vel = (Vector3){0.00, 0.00, 0.00};
	freeze.angular_vel = (Vector3){0.00, 0.00, 0.00};

	category_bits = 0b0001;
	collide_bits = 0b0000;

	select = false;
	static_state = false;
};

void Body::create(dWorldID world, dSpaceID space) {
	dBody = dBodyCreate(world);

	dBodySetPosition(
		dBody,
		position.x,
		position.y,
		position.z
	);

	dBodySetQuaternion(
		dBody,
		(dQuaternion){
		orientation.w,
		orientation.x,
		orientation.y,
		orientation.z,
	});
	
	switch(shape) {
		case Box: {
			dGeom = dCreateBox(space, sides.x, sides.y, sides.z);
			dMassSetBox(&mass, density, sides.x, sides.y, sides.z);
		} break;
		case Sphere: {
			dGeom = dCreateSphere(space, radius);
			dMassSetSphere(&mass, density, radius);
		} break;
		case Capsule: {
			dGeom = dCreateCapsule(space, radius, length);
			dMassSetCapsule(&mass, density, 1, length, radius);
		} break;
		case Cylinder: {
			dGeom = dCreateCylinder(space, radius, length);
			dMassSetCylinder(&mass, density, 1, length, radius);
		} break;
	}

	dMassAdjust(&mass, 0.50);
	dBodySetMass(dBody, &mass);
	dGeomSetBody(dGeom, dBody);
};

void Body::set_category_bits(unsigned long bits)
{
	dGeomSetCategoryBits(dGeom, bits);
};

void Body::set_category_bits()
{
	set_category_bits(category_bits);
};

void Body::set_collide_bits(unsigned long bits)
{
	dGeomSetCollideBits(dGeom, bits);
};

void Body::set_collide_bits()
{
	set_collide_bits(collide_bits);
};

void Body::make_static(dWorldID world) {
	if (static_state) {
		//dGeomSetBody(dGeom, 0);
		dJointID fixed = dJointCreateFixed(world ,0);
		dJointAttach(fixed, dBody, 0);
		dJointSetFixed(fixed);

		collide_bits = 0b0000;
		color = BLACK;
	}
};

void Body::update_freeze() {
	const dReal* linear_vel = dBodyGetLinearVel(dBody);
	const dReal* angular_vel = dBodyGetAngularVel(dBody);
	const dReal* position = dBodyGetPosition(dBody);
	const dReal* orientation = dBodyGetQuaternion(dBody);

	freeze.linear_vel = {linear_vel[0], linear_vel[1], linear_vel[2]};
	freeze.angular_vel = {angular_vel[0], angular_vel[1], angular_vel[2]};
	freeze.position = {position[0], position[1], position[2]};
	freeze.orientation = (Vector4){orientation[1], orientation[2], orientation[3], orientation[0]};
};

void Body::refreeze() {
	dBodySetLinearVel(
		dBody, 
		freeze.linear_vel.x,
		freeze.linear_vel.y,
		freeze.linear_vel.z
	);
	dBodySetAngularVel(
		dBody,
		freeze.angular_vel.x,
		freeze.angular_vel.y,
		freeze.angular_vel.z
	);

	dBodySetPosition(
		dBody,
		freeze.position.x,
		freeze.position.y,
		freeze.position.z
	);
	dBodySetQuaternion(
		dBody,
		(dQuaternion){
		freeze.orientation.w,
		freeze.orientation.x,
		freeze.orientation.y,
		freeze.orientation.z,
	});
};

void Body::reset()
{
	dBodySetLinearVel(dBody, 0.00, 0.00, 0.00);
	dBodySetAngularVel(dBody, 0.00, 0.00, 0.00);

	dBodySetPosition(
		dBody,
		position.x,
		position.y,
		position.z
	);

	dBodySetQuaternion(
		dBody,
		(dQuaternion) {
		orientation.w,
		orientation.x,
		orientation.y,
		orientation.z,
	});

	freeze.linear_vel = {0.00, 0.00, 0.00};
	freeze.angular_vel = {0.00, 0.00, 0.00};

	freeze.position = position;
	freeze.orientation = orientation;

}

void Body::draw_object(Color draw_color) {
	switch(shape) {
		case Box: {
			DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, sides.x, sides.y, sides.z, draw_color);
			DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, sides.x, sides.y, sides.z, BLACK);
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

void Body::draw(Color draw_color) {
	float angle;
	Vector3 axis;
	const dReal* dQ = dBodyGetQuaternion(dBody);
	QuaternionToAxisAngle(
		(Quaternion) { dQ[1], dQ[2], dQ[3], dQ[0] },
		&axis,
		&angle
	);

	const dReal *pos = dBodyGetPosition(dBody);
	rlPushMatrix();
	rlTranslatef(pos[0], pos[1], pos[2]);
	rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

	draw_object(draw_color);
};

void Body::draw_freeze() {
	float angle;
	Vector3 axis;
	Quaternion q = {
		freeze.orientation.x,
		freeze.orientation.y,
		freeze.orientation.z,
		freeze.orientation.w
	};
	QuaternionToAxisAngle(q, &axis, &angle);
	rlPushMatrix();
	rlTranslatef(
		freeze.position.x,
		freeze.position.y,
		freeze.position.z
	);
	rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);
	
	if (select) {
		draw_object(select_color);
	} else {
		draw_object(color);
	}
};

void Body::draw_ghost() {
	if (ghost) {
		draw(ghost_color);
	}
};

void Body::draw(bool freeze) {
	if (freeze) {
		draw_freeze();
		draw_ghost();
	} else {
		draw(color);
	}
};

void Body::toggle_ghost() {
	ghost = ghost == false;
};

std::string Body::get_name() {
	return m_name;
}

RayCollision Body::collide_mouse_ray(Ray ray, RayCollision collision) {
	switch(shape) {
		case Box: {
			collision = GetRayCollisionBox(ray,
				(BoundingBox) {
					(Vector3){
						freeze.position.x - 0.5f * sides.x,
						freeze.position.y - 0.5f * sides.y,
						freeze.position.z - 0.5f * sides.z,
					},
					(Vector3){
						freeze.position.x + 0.5f * sides.x,
						freeze.position.y + 0.5f * sides.y,
						freeze.position.z + 0.5f * sides.z,
					},
				}
			);
		} break;
		case Sphere: {
			collision = GetRayCollisionSphere(ray,
				(Vector3){
					freeze.position.x,
					freeze.position.y,
					freeze.position.z,
				},
				radius
			);
		} break;
		case Capsule: {
			collision = GetRayCollisionBox(ray,
				(BoundingBox) {
					(Vector3){
						freeze.position.x - 0.5f * sides.x,
						freeze.position.y - 0.5f * sides.y,
						freeze.position.z - 0.5f * sides.z,
					},
					(Vector3){
						freeze.position.x + 0.5f * sides.x,
						freeze.position.y + 0.5f * sides.y,
						freeze.position.z + 0.5f * sides.z,
					},
				}
			);
		} break;
		case Cylinder: {
			collision = GetRayCollisionBox(ray,
				(BoundingBox) {
					(Vector3){
						freeze.position.x - 0.5f * sides.x,
						freeze.position.y - 0.5f * sides.y,
						freeze.position.z - 0.5f * sides.z,
					},
					(Vector3){
						freeze.position.x + 0.5f * sides.x,
						freeze.position.y + 0.5f * sides.y,
						freeze.position.z + 0.5f * sides.z,
					},
				}
			);
		} break;
	}
	return collision;
};
