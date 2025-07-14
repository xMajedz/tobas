#include "player.h"

Player::Player(std::string_view name)
{
	m_name = name;

	m_b_color = WHITE;
	m_j_color = BLACK;
	m_g_color = m_j_color;
	m_g_color.a = 55;

	m_j_select_color = WHITE;

	use_engagepos = false;

	m_offset = {0.00, 0.00, 0.00};

	ghost = true;
};

std::string_view Player::GetName()
{
	return m_name;
};

JointState Player::GetJointState(JointID joint_id)
{
	return joint[joint_id].state;
};

JointState Player::GetJointStateAlt(JointID joint_id)
{
	return joint[joint_id].state_alt;
};

void Player::create(dWorldID world, dSpaceID space)
{
	for (auto& b : body) {
		b.color = m_b_color;
		b.ghost_color = m_g_color;

		b.ghost = true;
		b.static_state = false;

		b.position.x -= m_offset.x;
		b.position.y -= m_offset.y;
		b.position.z -= m_offset.z;

		if (use_engagepos) {
			b.position.x += engagepos.x;
			b.position.y += engagepos.y;
			b.position.z += engagepos.z;
			b.freeze.position = b.position;
		}

		b.create_dynamic(world, space);
		b.set_cat_bits(b_cat_bits);
		b.set_col_bits(b_col_bits);
	}

	for (auto& j : joint) {
		j.color = m_j_color;
		j.ghost_color = m_g_color;

		j.select_color = m_j_select_color;
		j.state = RELAX;
		j.state_alt = RELAX;

		j.ghost = true;
		j.static_state = false;

		j.position.x -= m_offset.x;
		j.position.y -= m_offset.y;
		j.position.z -= m_offset.z;

		if (use_engagepos) {
			j.position.x += engagepos.x;
			j.position.y += engagepos.y;
			j.position.z += engagepos.z;
			j.freeze.position = j.position;
		}

		j.create_joint(world, space, mass, body[j.connections[0]], body[j.connections[1]]);

		j.set_cat_bits(j_cat_bits);
		j.set_col_bits(j_col_bits);
	}
};

void Player::set_colors(Color b_color, Color j_color, Color g_color)
{
	m_b_color = b_color;
	m_j_color = j_color;
	m_g_color = g_color;
	m_g_color.a = 55;
};

void Player::set_offset(Vector3 offset)
{
	m_offset = offset;
};

void Player::set_offset()
{
	Vector3 sum = {0.00, 0.00, 0.00};
	for (auto& b : body) {
		sum.x += b.position.x;
		sum.y += b.position.y;
		sum.z += b.position.z;
	}

	b_count = body.size();

	for (auto& j : joint) {
		sum.x += j.position.x;
		sum.y += j.position.y;
		sum.z += j.position.z;
	}

	j_count = joint.size();

	set_offset((Vector3) {
		sum.x/(b_count + j_count),
		sum.y/(b_count + j_count),
		sum.z/(b_count + j_count)
	});
};

void Player::set_engageheight(float offset)
{
	for (auto& b : body) {
		b.position.z += offset;
		b.freeze.position.z += offset;
	}

	for (auto& j : joint) {
		j.position.z += offset;
		j.freeze.position.z += offset;
	}
};

void Player::set_engagedistance(float offset, float angle)
{
	Quaternion q = QuaternionFromMatrix(MatrixRotateZ(DEG2RAD * angle));
	for (auto& b : body) {
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

	for (auto& j : joint) {
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

void Player::set_col_bits(uint32_t b_bits, uint32_t j_bits) {
	b_col_bits = b_bits;
	j_col_bits = j_bits;
};

void Player::set_cat_bits(uint32_t b_bits, uint32_t j_bits) {
	b_cat_bits = b_bits;
	j_cat_bits = j_bits;
};

void Player::update_freeze() {
	for (auto& b : body) {
		b.update_freeze();
	}

	for (auto& j : joint) {
		j.update_joint_freeze();
	}
};

void Player::refreeze() {
	for (auto& b : body) {
		b.refreeze();
	}

	for (auto& j : joint) {
		j.refreeze_joint();
	}
};

void Player::reset() {
	for (auto& b : body) {
		b.reset();
	}
};

void Player::draw(bool freeze) {
	for (auto& b : body) {
		b.draw(freeze);
	}

	for (auto& j : joint) {
		j.draw_joint(freeze);
	}
};

void Player::toggle_ghost() {
	for (auto& b : body) {
		b.toggle_ghost();
	}

	for (auto& j : joint) {
		j.toggle_ghost();
	}
};


void Player::TriggerPlayerPassiveStates(PlayerPassiveStates state) {
	dReal strength = 0.00;
	for (auto& j : joint) {
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
	for (auto& j : joint) {
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
};
