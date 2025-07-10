#include "joint.h"

Joint::Joint() {}

Joint::Joint(const char* name)
{
	m_name = name;
}

void Joint::refreeze_joint() { };

void Joint::update_joint_freeze() {
	const dReal* pos = dGeomGetPosition(dGeom);
	const dReal* b_pos = dBodyGetPosition(dBody);
	const dReal* offset = dGeomGetOffsetPosition(dGeom);
	//const dReal *orientation = dBodyGetQuaternion(dBody);
	
	dQuaternion orientation;
	dGeomGetQuaternion(dGeom, orientation);
	freeze.orientation = (Vector4){orientation[1], orientation[2], orientation[3], orientation[0]};

	Vector3 v_offset = {
		b_pos[0] + offset[0],
		b_pos[1] + offset[1],
		b_pos[2] + offset[2],
	};

	//v_offset = Vector3RotateByQuaternion(v_offset, freeze.orientation);

	freeze.position = { pos[0], pos[1], pos[2]};
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

	Vector3 v_offset = {
		position.x - b1.position.x,
		position.y - b1.position.y,
		position.z - b1.position.z,
	};

	dGeomSetOffsetPosition(
		dGeom,
		v_offset.x,
		v_offset.y,
		v_offset.z
	);

	dGeomSetOffsetQuaternion(
		dGeom,
		(dQuaternion) {
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
				axis.x,
				axis.y,
				axis.z);
	
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
				axis.x,
				axis.y,
				axis.z);
	
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
				axis.x,
				axis.y,
				axis.z);
			dJointSetUniversalAxis2(
				dJoint,
				axis_alt.x,
				axis_alt.y,
				axis_alt.z);
	
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
		
			dJointSetHinge2Axes(dJoint,
				(dVector3){
				axis.x,
				axis.y,
				axis.z,
			},
				(dVector3){
				axis_alt.x,
				axis_alt.y,
				axis_alt.z,
			});
	
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

void Joint::draw_joint(bool freeze) {
	if (freeze) {
		draw_joint_freeze();
		draw_joint_ghost();
	} else {
		draw_joint(color);
	}
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
