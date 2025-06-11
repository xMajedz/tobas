#include <body.hpp>

Body::Body() {
	ghost_color = (Color){51, 51, 51, 51};
};

void Body::make_static() {
	if (static_state) {
		dJointID fixed = dJointCreateFixed(world ,0);
		dJointAttach(fixed, dBody, 0);
		dJointSetFixed(fixed);
	}
};

void Body::create() {
	dBody = dBodyCreate(world);
	dBodySetPosition(
		dBody,
		position[0],
		position[1],
		position[2]
	);

	dBodySetQuaternion(dBody, orientation);
	
	switch(shape) {
		case Box: {
			dGeom = dCreateBox(space, sides[0], sides[1], sides[2]);
			dMassSetBox(&mass, density, sides[0], sides[1], sides[2]);
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

	dMassAdjust(&mass, 0.5);
	dBodySetMass(dBody, &mass);
	dGeomSetBody(dGeom, dBody);

	dGeomSetCategoryBits(dGeom, category_bits);
	dGeomSetCollideBits(dGeom, collide_bits);
};

void Body::update_freeze() {
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

void Body::refreeze() {
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

void Body::draw_object(Color draw_color) {
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

void Body::draw(Color draw_color) {
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

void Body::draw_freeze() {
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

void Body::draw_ghost() {
	if (ghost) {
		draw(ghost_color);
	}
};

void Body::draw() {
	draw(color);
};

void Body::toggle_ghost() {
	ghost = ghost == false;
};

RayCollision Body::collide_mouse_ray(Ray ray, RayCollision collision) {
	switch(shape) {
		case Box: {
			collision = GetRayCollisionBox(ray,
				(BoundingBox) {
					(Vector3){
						freeze.position[0] - 0.5f * sides[0],
						freeze.position[1] - 0.5f * sides[1],
						freeze.position[2] - 0.5f * sides[2],
					},
					(Vector3){
						freeze.position[0] + 0.5f * sides[0],
						freeze.position[1] + 0.5f * sides[1],
						freeze.position[2] + 0.5f * sides[2],
					},
				}
			);
		} break;
		case Sphere: {
			collision = GetRayCollisionSphere(ray,
				(Vector3){
					freeze.position[0],
					freeze.position[1],
					freeze.position[2],
				},
				radius
			);
		} break;
		case Capsule: {
			collision = GetRayCollisionBox(ray,
				(BoundingBox) {
					(Vector3){
						freeze.position[0] - 0.5f * sides[0],
						freeze.position[1] - 0.5f * sides[1],
						freeze.position[2] - 0.5f * sides[2],
					},
					(Vector3){
						freeze.position[0] + 0.5f * sides[0],
						freeze.position[1] + 0.5f * sides[1],
						freeze.position[2] + 0.5f * sides[2],
					},
				}
			);
		} break;
		case Cylinder: {
			collision = GetRayCollisionBox(ray,
				(BoundingBox) {
					(Vector3){
						freeze.position[0] - 0.5f * sides[0],
						freeze.position[1] - 0.5f * sides[1],
						freeze.position[2] - 0.5f * sides[2],
					},
					(Vector3){
						freeze.position[0] + 0.5f * sides[0],
						freeze.position[1] + 0.5f * sides[1],
						freeze.position[2] + 0.5f * sides[2],
					},
				}
			);
		} break;
	}
	return collision;
};
