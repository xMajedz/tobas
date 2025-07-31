#include "body.h"

using namespace raylib;
#include "raymath.h"
#include "rlgl.h"

Body::Body(BodyID id, const char* name)
{
	m_id = id;
	m_name = name;

	dBody = nullptr;
	dGeom = nullptr;

	m_cat_bits = 0b0001;
	m_col_bits = 0b0000;

	m_color = BLACK;
	m_g_color = Fade(BLACK, 0.10);

	m_orientation = {0.00, 0.00, 0.00, 1.00};
	m_position = { 0 };
	m_offset =  { 0 };

	frame_orientation = {0.00, 0.00, 0.00, 1.00};
	frame_position = { 0 };
	frame_offset =  { 0 };

	frame_linear_vel = { 0 };
	frame_angular_vel = { 0 };

	freeze_orientation = {0.00, 0.00, 0.00, 1.00};
	freeze_position = { 0 };
	freeze_offset =  { 0 };

	freeze_linear_vel = { 0 };
	freeze_angular_vel = { 0 };

	active = false;
	m_active_color = BLACK;

	select = false;
	m_select_color = WHITE;

	m_static = false;
	m_composite = false;
	m_interactive = false;
}

void Body::Create(dWorldID world, dSpaceID space)
{
	m_world = world;
	m_space = space;

	frame_position = m_position;
	freeze_position = m_position;

	if (m_static) {
		CreateStatic();
	} else {
		CreateDynamic();
		m_col_bits = 0b0001;
	}

	SetCatBits();
	SetColBits();

	m_data.active = false;	
	dGeomSetData(dGeom, &m_data);
}

void Body::CreateBody()
{
	dBody = dBodyCreate(m_world);

	dBodySetPosition(
		dBody,
		m_position.x,
		m_position.y,
		m_position.z
	);

	dBodySetQuaternion(
		dBody,
		(dQuaternion){
		m_orientation.w,
		m_orientation.x,
		m_orientation.y,
		m_orientation.z,
	});

	dBodySetMass(dBody, &mass);
}

void Body::CreateGeom()
{
	switch(shape) {
	case BOX: {
		dGeom = dCreateBox(m_space, m_sides.x, m_sides.y, m_sides.z);
		dMassSetBox(&mass, density, m_sides.x, m_sides.y, m_sides.z);
	} break;
	case SPHERE: {
		dGeom = dCreateSphere(m_space, radius);
		dMassSetSphere(&mass, density, radius);
	} break;
	case CAPSULE: {
		dGeom = dCreateCapsule(m_space, radius, length);
		dMassSetCapsule(&mass, density, 1, length, radius);
	} break;
	case CYLINDER: {
		dGeom = dCreateCylinder(m_space, radius, length);
		dMassSetCylinder(&mass, density, 1, length, radius);
	} break;
	}
}

void Body::CreateDynamic()
{
	CreateGeom();
	CreateBody();
	dGeomSetBody(dGeom, dBody);
}

void Body::CreateStatic()
{
	CreateGeom();
	dGeomSetBody(dGeom, 0);
}

void Body::CreateComposite(dBodyID b)
{
	CreateGeom();
	dGeomSetBody(dGeom, b);
}

/*void Body::SetPosition(Vector3 position)
{
	dGeomSetPosition(dGeom, position, position, position);
}*/

void Body::SetColor(Color color)
{
	m_color = color;
}

BodyID Body::GetID()
{
	return m_id;
}

void Body::Step()
{
	if (!m_static && dBody != nullptr) {
		const dReal* linear_vel = dBodyGetLinearVel(dBody);
		const dReal* angular_vel = dBodyGetAngularVel(dBody);

		frame_linear_vel = { linear_vel[0], linear_vel[1], linear_vel[2] };
		frame_angular_vel = { angular_vel[0], angular_vel[1], angular_vel[2] };
	}

	dQuaternion orientation = { 0 };
	dGeomGetQuaternion(dGeom, orientation);
	frame_orientation = { orientation[1], orientation[2], orientation[3], orientation[0] };

	const dReal* position = dGeomGetPosition(dGeom);
	frame_position = { position[0], position[1], position[2] };

	const dReal* offset = dGeomGetOffsetPosition(dGeom); 
	frame_offset = { offset[0], offset[1], offset[2] };
}

void Body::SetCatBits(uint32_t bits)
{
	dGeomSetCategoryBits(dGeom, bits);
}

void Body::SetCatBits()
{
	SetCatBits(m_cat_bits);
}

void Body::SetColBits(uint32_t bits)
{
	dGeomSetCollideBits(dGeom, bits);
}

void Body::SetColBits()
{
	SetColBits(m_col_bits);
}

void Body::Freeze()
{
	if (!m_static) {
		const dReal* linear_vel = dBodyGetLinearVel(dBody);
		const dReal* angular_vel = dBodyGetAngularVel(dBody);

		freeze_linear_vel = {linear_vel[0], linear_vel[1], linear_vel[2]};
		freeze_angular_vel = {angular_vel[0], angular_vel[1], angular_vel[2]};
	}

	dQuaternion orientation = { 0 };
	dGeomGetQuaternion(dGeom, orientation);
	//freeze_orientation = (Vector4){orientation[1], orientation[2], orientation[3], orientation[0]};
	freeze_orientation = {orientation[1], orientation[2], orientation[3], orientation[0]};

	const dReal* position = dGeomGetPosition(dGeom);
	freeze_position = {position[0], position[1], position[2]};
}

void Body::Refreeze() {
	if (!m_static) {
		dBodySetLinearVel(
			dBody, 
			freeze_linear_vel.x,
			freeze_linear_vel.y,
			freeze_linear_vel.z
		);
		dBodySetAngularVel(
			dBody,
			freeze_angular_vel.x,
			freeze_angular_vel.y,
			freeze_angular_vel.z
		);
	}

	dGeomSetPosition(
		dGeom,
		freeze_position.x,
		freeze_position.y,
		freeze_position.z
	);
	dGeomSetQuaternion(
		dGeom,
		(dQuaternion){
		freeze_orientation.w,
		freeze_orientation.x,
		freeze_orientation.y,
		freeze_orientation.z,
	});
};

void Body::Reset()
{
	if (dBody != nullptr) {
		dBodySetLinearVel(dBody, 0.00, 0.00, 0.00);
		dBodySetAngularVel(dBody, 0.00, 0.00, 0.00);

		freeze_linear_vel = { 0 };
		freeze_angular_vel = { 0 };
	}

	dGeomSetPosition(
		dGeom,
		m_position.x,
		m_position.y,
		m_position.z
	);

	dGeomSetQuaternion(
		dGeom,
		(dQuaternion) {
		m_orientation.w,
		m_orientation.x,
		m_orientation.y,
		m_orientation.z,
	});

	frame_position = m_position;
	freeze_position = m_position;

	frame_orientation = m_orientation;
	freeze_orientation = m_orientation;
}

void Body::DrawObject(Color color)
{
	switch(shape) {
	case BOX: {
		DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, m_sides.x, m_sides.y, m_sides.z, color);
	} break;
	case SPHERE: {
		DrawSphere((Vector3){ 0.0f, 0.0f, 0.0f }, radius, color);
	} break;
	case CAPSULE: {
		DrawCapsule(
				(Vector3){ 0.0f, 0.0f, -(length/2) },
				(Vector3){ 0.0f, 0.0f,  (length/2) },
				radius,
				16,
				16,
				color
		);
	} break;
	case CYLINDER: {
		DrawCylinderEx(
				(Vector3){ 0.0f, 0.0f, -(length/2) },
				(Vector3){ 0.0f, 0.0f,  (length/2) },
				radius,
				radius,
				16,
				color
		);
	} break;
	}
}

void Body::DrawObjectWires(Color color)
{
	switch(shape) {
	case BOX: {
		DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, m_sides.x, m_sides.y, m_sides.z, color);
	} break;
	case SPHERE: {
		DrawSphereWires((Vector3){ 0.0f, 0.0f, 0.0f }, radius, 16, 16, color);
	} break;
	case CAPSULE: {
		DrawCapsuleWires(
				(Vector3){ 0.0f, 0.0f, -(length/2) },
				(Vector3){ 0.0f, 0.0f,  (length/2) },
				radius,
				16,
				16,
				color
		);
	} break;
	case CYLINDER: {
		DrawCylinderWiresEx(
				(Vector3){ 0.0f, 0.0f, -(length/2) },
				(Vector3){ 0.0f, 0.0f,  (length/2) },
				radius,
				radius,
				16,
				color
		);
	} break;
	}
}

void Body::Draw(Color color)
{
	Quaternion q = {
		frame_orientation.x,
		frame_orientation.y,
		frame_orientation.z,
		frame_orientation.w,
	};

	float angle;
	Vector3 axis;

	QuaternionToAxisAngle(q, &axis, &angle);

	rlPushMatrix();
	rlTranslatef(frame_position.x, frame_position.y, frame_position.z);
	rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

	DrawObject(color);
	rlPopMatrix();
}

void Body::DrawFreeze()
{
	Quaternion q = {
		freeze_orientation.x,
		freeze_orientation.y,
		freeze_orientation.z,
		freeze_orientation.w
	};

	float angle;
	Vector3 axis;

	QuaternionToAxisAngle(q, &axis, &angle);
	rlPushMatrix();
	rlTranslatef(
		freeze_position.x,
		freeze_position.y,
		freeze_position.z
	);

	rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);
	
	if (select) {
		DrawObject(m_select_color);
	}

	if (active) {
		DrawObject(m_active_color);
	}

	if (!select && !active) {
		DrawObject(m_color);
	}

	rlPopMatrix();
}

void Body::DrawGhost()
{
	if (ghost) {
		Draw(m_g_color);
	}
}

void Body::Draw(bool freeze)
{
	if (freeze) {
		DrawFreeze();
		DrawGhost();
	} else {
		Draw(m_color);
	}
}

void Body::ToggleGhost()
{
	ghost = ghost == false;
}

std::string Body::GetName()
{
	return m_name;
}

RayCollision Body::collide_mouse_ray(Ray ray, RayCollision collision)
{
	switch(shape) {
	case BOX: {
		collision = GetRayCollisionBox(ray,
			(BoundingBox) {
				(Vector3){
					freeze_position.x - 0.5f * m_sides.x,
					freeze_position.y - 0.5f * m_sides.y,
					freeze_position.z - 0.5f * m_sides.z,
				},
				(Vector3){
					freeze_position.x + 0.5f * m_sides.x,
					freeze_position.y + 0.5f * m_sides.y,
					freeze_position.z + 0.5f * m_sides.z,
				},
			}
		);
	} break;
	case SPHERE: {
		collision = GetRayCollisionSphere(ray,
			(Vector3){
				freeze_position.x,
				freeze_position.y,
				freeze_position.z,
			},
			radius
		);
	} break;
	case CAPSULE: {
		collision = GetRayCollisionBox(ray,
			(BoundingBox) {
				(Vector3){
					freeze_position.x - 0.5f * m_sides.x,
					freeze_position.y - 0.5f * m_sides.y,
					freeze_position.z - 0.5f * m_sides.z,
				},
				(Vector3){
					freeze_position.x + 0.5f * m_sides.x,
					freeze_position.y + 0.5f * m_sides.y,
					freeze_position.z + 0.5f * m_sides.z,
				},
			}
		);
	} break;
	case CYLINDER: {
		collision = GetRayCollisionBox(ray,
			(BoundingBox) {
				(Vector3){
					freeze_position.x - 0.5f * m_sides.x,
					freeze_position.y - 0.5f * m_sides.y,
					freeze_position.z - 0.5f * m_sides.z,
				},
				(Vector3){
					freeze_position.x + 0.5f * m_sides.x,
					freeze_position.y + 0.5f * m_sides.y,
					freeze_position.z + 0.5f * m_sides.z,
				},
			}
		);
	} break;
	}

	return collision;
}

Joint::Joint(JointID id, const char* name)
{
	m_id = id;
	m_name = name;
	state = RELAX;
	state_alt = RELAX;
}

void Joint::Create(dWorldID world, dSpaceID space, Body b1, Body b2)
{
	m_world = world;
	m_space = space;
	if (m_composite) {
		m_offset = {
			m_position.x - b1.m_position.x,
			m_position.y - b1.m_position.y,
			m_position.z - b1.m_position.z,
		};

		CreateComposite(b1.dBody);
		dBody = b1.dBody;

		dGeomSetOffsetWorldPosition(dGeom, m_position.x, m_position.y, m_position.z);
	}
	
	switch(type)
	{
	case HINGE: {
		dJoint = dJointCreateHinge(world, 0);
		dJointAttach(dJoint, b1.dBody, b2.dBody);
		dJointSetHingeAnchor(
			dJoint,
			m_position.x,
			m_position.y,
			m_position.z);
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
	case dSLIDER: {
		dJoint = dJointCreateSlider(world, 0);
		dJointAttach(dJoint, b1.dBody, b2.dBody);
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
	case UNIVERSAL: {
		dJoint = dJointCreateUniversal(world, 0);
		dJointAttach(dJoint, b1.dBody, b2.dBody);
		dJointSetUniversalAnchor(
			dJoint,
			m_position.x,
			m_position.y,
			m_position.z);

		dJointSetUniversalAnchor(
			dJoint,
			m_position.x,
			m_position.y,
			m_position.z);

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
	case HINGE2: {
		dJoint = dJointCreateHinge2(world, 0);
		dJointAttach(dJoint, b1.dBody, b2.dBody);
		dJointSetHinge2Anchor(
			dJoint,
			m_position.x,
			m_position.y,
			m_position.z);

		dJointSetHinge2Anchor(
			dJoint,
			m_position.x,
			m_position.y,
			m_position.z);
	
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
		dJointAttach(dJoint, b1.dBody, b2.dBody);
		dJointSetFixed(dJoint);
	}

	if (m_composite) {
		SetCatBits();
		SetColBits();
	}
}

void Joint::Draw(Color color)
{
	Quaternion q = {
		frame_orientation.x,
		frame_orientation.y,
		frame_orientation.z,
		frame_orientation.w,
	};

	float angle;
	Vector3 axis;
	QuaternionToAxisAngle(q, &axis, &angle);

	rlPushMatrix();
	rlTranslatef(frame_position.x, frame_position.y, frame_position.z);
	rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);

	DrawObject(color);

	rlPopMatrix();
}

void Joint::Draw(bool freeze)
{
	if (!m_composite) return;
	if (freeze) {
		DrawGhost();
		DrawFreeze();
	} else {
		Draw(m_color);
	}

	if (select) {
		DrawAxis(freeze);
		DrawRange(freeze);
	}
}

void Joint::DrawAxis(bool freeze)
{
	Vector3 v_axis = Vector3Normalize((Vector3){axis.x, axis.y, axis.z});
	if (freeze) {
		Vector3 start = {
			freeze_position.x + v_axis.x * 0.2,
			freeze_position.y + v_axis.y * 0.2,
			freeze_position.z + v_axis.z * 0.2,
		};
	
		Vector3 end = {
			freeze_position.x - v_axis.x * 0.2,
			freeze_position.y - v_axis.y * 0.2,
			freeze_position.z - v_axis.z * 0.2,
		};

		DrawLine3D(start, end, BLACK);
	} else {
		Vector3 start = {
			frame_position.x + v_axis.x * 0.2,
			frame_position.y + v_axis.y * 0.2,
			frame_position.z + v_axis.z * 0.2,
		};
	
		Vector3 end = {
			frame_position.x - v_axis.x * 0.2,
			frame_position.y - v_axis.y * 0.2,
			frame_position.z - v_axis.z * 0.2,
		};

		DrawLine3D(start, end, BLACK);
	}
}

void Joint::DrawRange(bool freeze)
{
	Vector3 v_axis = Vector3Normalize((Vector3){axis.x, axis.y, axis.z});
	Vector3 v = Vector3Perpendicular(v_axis);
	if (freeze) {
		Vector3 start = {
			freeze_position.x + v.x * 0.2,
			freeze_position.y + v.y * 0.2,
			freeze_position.z + v.z * 0.2,
		};
	
		Vector3 end = {
			freeze_position.x - v.x * 0.2,
			freeze_position.y - v.y * 0.2,
			freeze_position.z - v.z * 0.2,
		};

		DrawLine3D(start, end, RED);
	} else {
		Vector3 start = {
			frame_position.x + v.x * 0.2,
			frame_position.y + v.y * 0.2,
			frame_position.z + v.z * 0.2,
		};
	
		Vector3 end = {
			frame_position.x - v.x * 0.2,
			frame_position.y - v.y * 0.2,
			frame_position.z - v.z * 0.2,
		};

		DrawLine3D(start, end, RED);
	}
}

void Joint::TriggerActiveStateAlt(dReal direction)
{
	switch(type)
	{
	case UNIVERSAL: {
		dJointSetUniversalParam(dJoint, dParamFMax2, strength_alt);
		dJointSetUniversalParam(dJoint, dParamVel2, direction * velocity_alt);
	} break;
	case HINGE2: {
		dJointSetHinge2Param(dJoint, dParamFMax2, strength_alt);
		dJointSetHinge2Param(dJoint, dParamVel2, direction * velocity_alt);
	} break;
	}
}

void Joint::TriggerPassiveStateAlt(dReal strength)
{
	switch(type) {
	case UNIVERSAL: {
		dJointSetUniversalParam(dJoint, dParamFMax2, strength);
		dJointSetUniversalParam(dJoint, dParamVel2, 0.00);
	} break;
	case HINGE2: {
		dJointSetHinge2Param(dJoint, dParamFMax2, strength);
		dJointSetHinge2Param(dJoint, dParamVel2, 0.00);
	} break;
	}
}

void Joint::TriggerActiveState(dReal direction)
{
	switch(type)
	{
	case HINGE: {
		dJointSetHingeParam(dJoint, dParamFMax, strength);
		dJointSetHingeParam(dJoint, dParamVel, direction * velocity);
	} break;
	case dSLIDER: {
		dJointSetSliderParam(dJoint, dParamFMax, strength);
		dJointSetSliderParam(dJoint, dParamVel, direction * velocity);
	} break;
	case UNIVERSAL: {
		dJointSetUniversalParam(dJoint, dParamFMax, strength);
		dJointSetUniversalParam(dJoint, dParamVel, direction * velocity);
	} break;
	case HINGE2: {
		dJointSetHinge2Param(dJoint, dParamFMax, strength);
		dJointSetHinge2Param(dJoint, dParamVel, direction * velocity);
	} break;
	}
}

void Joint::TriggerPassiveState(dReal strength)
{
	switch(type)
	{
	case HINGE: {
		dJointSetHingeParam(dJoint, dParamFMax, strength);
		dJointSetHingeParam(dJoint, dParamVel, 0.00);
	} break;
	case dSLIDER: {
		dJointSetSliderParam(dJoint, dParamFMax, strength);
		dJointSetSliderParam(dJoint, dParamVel, 0.00);
	} break;
	case UNIVERSAL: {
		dJointSetUniversalParam(dJoint, dParamFMax, strength);
		dJointSetUniversalParam(dJoint, dParamVel, 0.00);
	} break;
	case HINGE2: {
		dJointSetHinge2Param(dJoint, dParamFMax, strength);
		dJointSetHinge2Param(dJoint, dParamVel, 0.00);
	} break;
	}
}

void Joint::TogglePassiveState()
{
	if (state == RELAX) {
		state = HOLD;
		TriggerPassiveState(strength);
	} else {
		state = RELAX;
		TriggerPassiveState(0.00);
	}
}

void Joint::TogglePassiveStateAlt()
{
	if (state_alt == RELAX) {
		state_alt = HOLD;
		TriggerPassiveStateAlt(strength);
	} else {
		state_alt = RELAX;
		TriggerPassiveStateAlt(0.00);
	}
}

void Joint::ToggleActiveState()
{
	if (state == FORWARD) {
		state = BACKWARD;
		TriggerActiveState(-1.00);
	} else {
		state = FORWARD;
		TriggerActiveState(1.00);
	}
}

void Joint::ToggleActiveStateAlt()
{
	if (state_alt == FORWARD) {
		state_alt = BACKWARD;
		TriggerActiveStateAlt(-1.00);
	} else {
		state_alt = FORWARD;
		TriggerActiveStateAlt(1.00);
	}
}

void Joint::CycleState()
{
	switch(state)
	{
	case FORWARD: {
		state = BACKWARD;
		TriggerActiveState(-1.00);
	} break;
	case BACKWARD: {
		state = HOLD;
		TriggerPassiveState(strength);
	} break;
	case HOLD: {
		state = RELAX;
		TriggerPassiveState(0.00);
	} break;
	default:
		state = FORWARD;
		TriggerActiveState(1.00);
	}
}

void Joint::CycleStateAlt()
{
	switch(state_alt)
	{
	case FORWARD: {
		state_alt = BACKWARD;
		TriggerActiveStateAlt(-1.00);
	} break;
	case BACKWARD: {
		state_alt = HOLD;
		TriggerPassiveStateAlt(strength_alt);
	} break;
	case HOLD: {
		state_alt = RELAX;
		TriggerPassiveStateAlt(0.00);
	} break;
	default:
		state_alt = FORWARD;
		TriggerActiveStateAlt(1.00);
	}
}
