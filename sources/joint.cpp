#include <joint.h>

#include <iostream>
#define PRINT(X) std::cout << X << std::endl;

void Joint::refreeze_joint() {
	dGeomSetPosition(
		dGeom,
		freeze.position.x,
		freeze.position.y,
		freeze.position.z
	);
	dGeomSetQuaternion(dGeom, (dQuaternion) {
		freeze.orientation.w,
		freeze.orientation.x,
		freeze.orientation.y,
		freeze.orientation.z,
	});

	dBodySetLinearVel(
		dBody,
		freeze.linear_vel.x,
		freeze.linear_vel.y,
		freeze.linear_vel.z);
	dBodySetAngularVel(
		dBody,
		freeze.angular_vel.x,
		freeze.angular_vel.y,
		freeze.angular_vel.z);
};

void Joint::update_joint_freeze() {
	dQuaternion orientation;
	dGeomGetQuaternion(dGeom, orientation);

	const dReal *position = dGeomGetPosition(dGeom);
	freeze.position.x = position[0];
	freeze.position.y = position[1];
	freeze.position.z = position[2];

	freeze.orientation.w = orientation[0];
	freeze.orientation.x = orientation[1];
	freeze.orientation.y = orientation[2];
	freeze.orientation.z = orientation[3];

	const dReal *linear_vel = dBodyGetLinearVel(dBody);
	const dReal *angular_vel = dBodyGetAngularVel(dBody);

	freeze.linear_vel.x = linear_vel[0];
	freeze.linear_vel.y = linear_vel[1];
	freeze.linear_vel.z = linear_vel[2];

	freeze.angular_vel.x = angular_vel[0];
	freeze.angular_vel.y = angular_vel[1];
	freeze.angular_vel.z = angular_vel[2];
};

void Joint::create_joint(dWorldID world, dSpaceID space, dMass mass, Body b1, Body b2) {
	dBody = b1.dBody;

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

	dGeomSetBody(dGeom, dBody);
	
	dGeomSetOffsetPosition(dGeom,
		position.x - b1.position.x,
		position.y - b1.position.y,
		position.z - b1.position.z
	);

	dGeomSetOffsetQuaternion(dGeom, (dQuaternion) {
		orientation.w,
		orientation.x,
		orientation.y,
		orientation.z,
	});

	switch(connectionType) {
		case Hinge: {
			dJoint = dJointCreateHinge(world, 0);
			dJointAttach(dJoint, dBody, b2.dBody);
			dJointSetHingeAnchor(
				dJoint,
				position.x,
				position.y,
				position.z);
			dJointSetHingeAxis(
				dJoint,
				axis[0],
				axis[1],
				axis[2]);
	
			dJointSetHingeParam(
				dJoint,
				dParamHiStop,
				range[0]);
			dJointSetHingeParam(
				dJoint,
				dParamLoStop,
				range[1]);
		} break;
		case Slider: {
			dJoint = dJointCreateSlider(world, 0);
			dJointAttach(dJoint, dBody, b2.dBody);
			dJointSetSliderAxis(
				dJoint,
				axis[0],
				axis[1],
				axis[2]);
	
			dJointSetSliderParam(
				dJoint,
				dParamHiStop,
				range[0]);
			dJointSetSliderParam(
				dJoint,
				dParamLoStop,
				range[1]);
		} break;
		case Universal: {
			dJoint = dJointCreateUniversal(world, 0);
			dJointAttach(dJoint, dBody, b2.dBody);
			dJointSetUniversalAnchor(
				dJoint,
				position.x,
				position.y,
				position.z);
	
			dJointSetUniversalAnchor(
				dJoint,
				position.x,
				position.y,
				position.z);
	
			dJointSetUniversalAxis1(
				dJoint,
				axis[0],
				axis[1],
				axis[2]);
			dJointSetUniversalAxis2(
				dJoint,
				axis_alt[0],
				axis_alt[1],
				axis_alt[2]);
	
			dJointSetUniversalParam(
				dJoint,
				dParamHiStop,
				range[0]);
	
			dJointSetUniversalParam(
				dJoint,
				dParamHiStop2,
				range_alt[0]);
	
			dJointSetUniversalParam(
				dJoint,
				dParamLoStop,
				range[1]);
	
			dJointSetUniversalParam(
				dJoint,
				dParamLoStop2,
				range_alt[1]);
		} break;
		case Hinge2: {
			dJoint = dJointCreateHinge2(world, 0);
			dJointAttach(dJoint, dBody, b2.dBody);
			dJointSetHinge2Anchor(
				dJoint,
				position.x,
				position.y,
				position.z);
	
			dJointSetHinge2Anchor(
				dJoint,
				position.x,
				position.y,
				position.z);
		
			dJointSetHinge2Axes(dJoint, axis, axis_alt);
	
			dJointSetHinge2Param(
				dJoint,
				dParamHiStop,
				range[0]);
	
			dJointSetHinge2Param(
				dJoint,
				dParamHiStop2,
				range_alt[0]);
	
			dJointSetHinge2Param(
				dJoint,
				dParamLoStop,
				range[1]);
	
			dJointSetHinge2Param(
				dJoint,
				dParamLoStop2,
				range_alt[1]);
		} break;
		default:
			dJoint = dJointCreateFixed(world, 0);
			dJointAttach(dJoint, dBody, b2.dBody);
			dJointSetFixed(dJoint);
	}

	dGeomSetCategoryBits(dGeom, category_bits);
	dGeomSetCollideBits(dGeom, collide_bits);
};

void Joint::draw_joint(Color draw_color) {
	dQuaternion dQ;
	dRtoQ(dGeomGetRotation(dGeom), dQ);

	float angle;
	Vector3 axis;
	Quaternion q = { dQ[1], dQ[2], dQ[3], dQ[0] };
	QuaternionToAxisAngle(q, &axis, &angle);

	const dReal *position = dGeomGetPosition(dGeom);
	rlPushMatrix();
	rlTranslatef(position[0], position[1], position[2]);
	rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

	draw_object(draw_color);
};

void Joint::draw_joint_freeze() {
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

void Joint::draw_joint_ghost() {
	if (ghost) {
		draw(ghost_color);
	}
};

void Joint::draw_joint() {
	draw_joint(color);
};

void Joint::TriggerActiveStateAlt(dReal direction) {
	switch(connectionType) {
		case Hinge: {
		} break;
		case Slider: {
		} break;
		case Universal: {
			dJointSetUniversalParam(dJoint, dParamFMax2, strength_alt);
			dJointSetUniversalParam(dJoint, dParamVel2, direction * velocity_alt);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(dJoint, dParamFMax2, strength_alt);
			dJointSetHinge2Param(dJoint, dParamVel2, direction * velocity_alt);
		} break;
	}
};

void Joint::TriggerPassiveStateAlt(dReal strength) {
	switch(connectionType) {
		case Hinge: {
		} break;
		case Slider: {
		} break;
		case Universal: {
			dJointSetUniversalParam(dJoint, dParamFMax2, strength);
			dJointSetUniversalParam(dJoint, dParamVel2, 0.00);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(dJoint, dParamFMax2, strength);
			dJointSetHinge2Param(dJoint, dParamVel2, 0.00);
		} break;
	}
};

void Joint::TriggerActiveState(dReal direction) {
	switch(connectionType) {
		case Hinge: {
			dJointSetHingeParam(dJoint, dParamFMax, strength);
			dJointSetHingeParam(dJoint, dParamVel, direction * velocity);
		} break;
		case Slider: {
			dJointSetSliderParam(dJoint, dParamFMax, strength);
			dJointSetSliderParam(dJoint, dParamVel, direction * velocity);
		} break;
		case Universal: {
			dJointSetUniversalParam(dJoint, dParamFMax, strength);
			dJointSetUniversalParam(dJoint, dParamVel, direction * velocity);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(dJoint, dParamFMax, strength);
			dJointSetHinge2Param(dJoint, dParamVel, direction * velocity);
		} break;
	}
};

void Joint::TriggerPassiveState(dReal strength) {
	switch(connectionType) {
		case Hinge: {
			dJointSetHingeParam(dJoint, dParamFMax, strength);
			dJointSetHingeParam(dJoint, dParamVel, 0.00);
		} break;
		case Slider: {
			dJointSetSliderParam(dJoint, dParamFMax, strength);
			dJointSetSliderParam(dJoint, dParamVel, 0.00);
		} break;
		case Universal: {
			dJointSetUniversalParam(dJoint, dParamFMax, strength);
			dJointSetUniversalParam(dJoint, dParamVel, 0.00);
		} break;
		case Hinge2: {
			dJointSetHinge2Param(dJoint, dParamFMax, strength);
			dJointSetHinge2Param(dJoint, dParamVel, 0.00);
		} break;
	}
};

void Joint::TogglePassiveState() {
	if (state == RELAX) {
		state = HOLD;
		TriggerPassiveState(strength);
	} else {
		state = RELAX;
		TriggerPassiveState(0.00);
	}
};

void Joint::TogglePassiveStateAlt() {
	if (state_alt == RELAX) {
		state_alt = HOLD;
		TriggerPassiveStateAlt(strength);
	} else {
		state_alt = RELAX;
		TriggerPassiveStateAlt(0.00);
	}
};

void Joint::ToggleActiveState() {
	if (state == FORWARD) {
		state = BACKWARD;
		TriggerActiveState(-1.00);
	} else {
		state = FORWARD;
		TriggerActiveState(1.00);
	}
};

void Joint::ToggleActiveStateAlt() {
	if (state_alt == FORWARD) {
		state_alt = BACKWARD;
		TriggerActiveStateAlt(-1.00);
	} else {
		state_alt = FORWARD;
		TriggerActiveStateAlt(1.00);
	}
};

void Joint::CycleState() {
	if (state == FORWARD) {
		state = BACKWARD;
		TriggerActiveState(-1.00);
	} else if (state == BACKWARD) {
		state = HOLD;
		TriggerPassiveState(strength);
	} else if (state == HOLD) {
		state = RELAX;
		TriggerPassiveState(0.00);
	} else {
		state = FORWARD;
		TriggerActiveState(1.00);
	}
};

void Joint::CycleStateAlt() {	
	if (state_alt == FORWARD) {
		state_alt = BACKWARD;
		TriggerActiveStateAlt(-1.00);
	} else if (state_alt == BACKWARD) {
		state_alt = HOLD;
		TriggerPassiveStateAlt(strength_alt);
	} else if (state_alt == HOLD) {
		state_alt = RELAX;
		TriggerPassiveStateAlt(0.00);
	} else {
		state_alt = FORWARD;
		TriggerActiveStateAlt(1.00);
	}
};
