#include "player.h"

Player::Player(const char* name)
{
	m_name = name;
}

Player::Player()
{
	body_color = WHITE;
	joint_select_color = WHITE;
	joint_color = BLACK;
	ghost_color = joint_color;
	ghost_color.a = 55;

	m_offset = {0.00, 0.00, 0.00};

	ghost = true;
};

std::string Player::get_name()
{
	return m_name;
}

void Player::create(dWorldID world, dSpaceID space)
{
	for (int i = 0; i < body.length; i += 1) {
		auto&& b = body[i];
		b.color = body_color;
		b.ghost_color = ghost_color;

		b.ghost = true;
		b.static_state = false;
		if (use_engagepos) {
			b.position.x += engagepos.x;
			b.position.y += engagepos.y;
			b.position.z += engagepos.z;
			b.freeze.position = b.position;
		}

		b.create(world, space);
		b.set_category_bits(body_category_bits);
		b.set_collide_bits(body_collide_bits);
	}

	for (int i = 0; i < joint.length; i += 1) {
		auto&& j = joint[i];
		j.color = joint_color;
		j.ghost_color = ghost_color;

		j.select_color = joint_select_color;
		j.state = RELAX;
		j.state_alt = RELAX;

		j.ghost = true;
		j.static_state = false;

		if (use_engagepos) {
			j.position.x += engagepos.x;
			j.position.y += engagepos.y;
			j.position.z += engagepos.z;
			j.freeze.position = j.position;
		}

		//j.create_joint(world, space, mass, body[j.connections[0]], body[j.connections[1]]);
		//j.create_joint(world, space, mass, body[0], body[1]);

		//j.set_category_bits(joint_category_bits);
		//j.set_collide_bits(joint_collide_bits);
	}
};

void Player::set_offset(Vector3 offset)
{
	m_offset = offset;
};

void Player::set_offset()
{
	Vector3 sum = {0.00, 0.00, 0.00};
	for (int i = 0; i < body.length; i += 1) {
		auto&& b = body[i];
		sum.x += b.position.x;
		sum.y += b.position.y;
		sum.z += b.position.z;
	}

	for (int i = 0; i < joint.length; i += 1) {
		auto&& j = joint[i];
		sum.x += j.position.x;
		sum.y += j.position.y;
		sum.z += j.position.z;
	}

	set_offset((Vector3) {
		sum.x/(b_count + j_count),
		sum.y/(b_count + j_count),
		sum.z/(b_count + j_count)
	});
};

void Player::set_engageheight(float offset)
{
	for (int i = 0; i < body.length; i += 1) {
		auto&& b = body[i];
		b.position.z += offset;
		b.freeze.position.z += offset;
	}

	for (int i = 0; i < joint.length; i += 1) {
		auto&& j = joint[i];
		j.position.z += offset;
		j.freeze.position.z += offset;
	}
};

void Player::set_engagedistance(float offset, float angle)
{
	Quaternion q = QuaternionFromMatrix(MatrixRotateZ(DEG2RAD * angle));
	for (int i = 0; i < body.length; i += 1) {
		auto&& b = body[i];
		Vector3 offsetv = {
			b.position.x - m_offset.x,
			b.position.y - m_offset.y + offset,
			b.position.z - m_offset.z,
		};
		offsetv = Vector3RotateByQuaternion(offsetv, q);
		b.position = offsetv;
		b.position.x += m_offset.x;
		b.position.y += m_offset.y;
		b.position.z += m_offset.z;
		b.freeze.position = offsetv;
		b.orientation = QuaternionMultiply(q, b.orientation);
		b.freeze.orientation = b.orientation;
	}

	for (int i = 0; i < joint.length; i += 1) {
		auto&& j = joint[i];
		Vector3 offsetv = {
			j.position.x - m_offset.x,
			j.position.y - m_offset.y + offset,
			j.position.z - m_offset.z,
		};
		offsetv = Vector3RotateByQuaternion(offsetv, q);
		j.position = offsetv;
		j.position.x += m_offset.x;
		j.position.y += m_offset.y;
		j.position.z += m_offset.z;
		j.freeze.position = offsetv;
		j.orientation = QuaternionMultiply(q, j.orientation);
		j.freeze.orientation = j.orientation;
	}
}

void Player::set_collide_bits(unsigned long b_bits, unsigned long j_bits) {
		body_collide_bits = b_bits;
		joint_collide_bits = j_bits;
};

void Player::set_category_bits(unsigned long b_bits, unsigned long j_bits) {
		body_category_bits = b_bits;
		joint_category_bits = j_bits;
};

void Player::update_freeze() {
	for (int i = 0; i < body.length; i += 1) {
		auto&& b = body[i];
		b.update_freeze();
	}

	for (int i = 0; i < joint.length; i += 1) {
		auto&& j = joint[i];
		//j.update_joint_freeze();
	}
};

void Player::refreeze() {
	for (int i = 0; i < body.length; i += 1) {
		auto&& b = body[i];
		b.refreeze();
	}

	for (int i = 0; i < joint.length; i += 1) {
		auto&& j = joint[i];
		//j.refreeze_joint();
	}
};

void Player::reset() {
	for (int i = 0; i < body.length; i += 1) {
		auto&& b = body[i];
		b.reset();
	}
};

void Player::draw() {
	for (int i = 0; i < body.length; i += 1) {
		auto&& b = body[i];
		b.draw();
	}

	for (int i = 0; i < joint.length; i += 1) {
		auto&& j = joint[i];
		//j.draw_joint();
	}
};

void Player::draw_freeze() {

	for (int i = 0; i < body.length; i += 1) {
		auto&& b = body[i];
		b.draw_freeze();
	}

	for (int i = 0; i < joint.length; i += 1) {
		auto&& j = joint[i];
		//j.draw_joint_freeze();
	}
};

void Player::draw_ghost() {
	if (ghost) {
		for (int i = 0; i < body.length; i += 1) {
			auto&& b = body[i];
			b.draw_ghost();
		}

		for (int i = 0; i < joint.length; i += 1) {
			auto&& j = joint[i];
			//j.draw_joint_ghost();
		}
	}
};
/*
void Player::toggle_ghost() {
	for (auto& [body_name, b] : body) {
		b.toggle_ghost();
	}

	for (auto& [joint_name, j] : joint) {
		j.toggle_ghost();
	}
};

void Player::TriggerPlayerPassiveStates(PlayerPassiveStates state) {
	dReal strength = 0.00;
	for (auto& [joint_name, j] : joint) {
		j.state = RELAX;
		if (state == HOLD_ALL) {
			strength = j.strength;
			j.state = HOLD;
		}
		switch(j.connectionType) {
			case Hinge: {
				dJointSetHingeParam(j.dJoint, dParamFMax, strength);
				dJointSetHingeParam(j.dJoint, dParamVel, 0.0f);
			} break;
			case Slider: {
				dJointSetSliderParam(j.dJoint, dParamFMax, strength);
				dJointSetSliderParam(j.dJoint, dParamVel, 0.0f);
			} break;
			case Universal: {
				dJointSetUniversalParam(j.dJoint, dParamFMax, strength);
				dJointSetUniversalParam(j.dJoint, dParamVel, 0.0f);
			} break;
			case Hinge2: {
				dJointSetHinge2Param(j.dJoint, dParamFMax, strength);
				dJointSetHinge2Param(j.dJoint, dParamVel, 0.0f);
			} break;
		}
	}
};

void Player::TriggerPlayerPassiveStatesAlt(PlayerPassiveStates state) {
	dReal strength = 0.00;
	for (auto& [joint_name, j] : joint) {
		j.state_alt = RELAX;
		if (state == HOLD_ALL) {
			strength = j.strength_alt;
			j.state_alt = HOLD;
		}
		switch(j.connectionType) {
			case Hinge: {
			} break;
			case Slider: {
			} break;
			case Universal: {
				dJointSetUniversalParam(j.dJoint, dParamFMax2, strength);
				dJointSetUniversalParam(j.dJoint, dParamVel2, 0.0f);
			} break;
			case Hinge2: {
				dJointSetHinge2Param(j.dJoint, dParamFMax2, strength);
				dJointSetHinge2Param(j.dJoint, dParamVel2, 0.0f);
			} break;
		}
	}
}

void Player::TogglePlayerPassiveStates() {
	refreeze();
	if (passive_states == RELAX_ALL) {
		TriggerPlayerPassiveStates(HOLD_ALL);
		passive_states = HOLD_ALL;
	} else {
		TriggerPlayerPassiveStates(RELAX_ALL);
		passive_states = RELAX_ALL;
	}
}

void Player::TogglePlayerPassiveStatesAlt() {
	refreeze();
	if (passive_states_alt == RELAX_ALL) {
		TriggerPlayerPassiveStatesAlt(HOLD_ALL);
		passive_states_alt = HOLD_ALL;
	} else {
		TriggerPlayerPassiveStatesAlt(RELAX_ALL);
		passive_states_alt = RELAX_ALL;
	}
}

void Player::RelaxAll() {
	refreeze();
	TriggerPlayerPassiveStates(RELAX_ALL);
	passive_states = RELAX_ALL;
};

void Player::RelaxAllAlt() {
	refreeze();
	TriggerPlayerPassiveStatesAlt(RELAX_ALL);
	passive_states_alt = RELAX_ALL;
};*/
