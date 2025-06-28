#include <player.h>

#include <iostream>
#define PRINT(X) std::cout << "-- TEST " << X << " --" << std::endl;
#define PRINT_A PRINT("A")
#define PRINT_B PRINT("B")
#define PRINT_C PRINT("C")

Player::Player() {
	body_color = (Color){ 255, 255, 255, 255 };
	joint_color = BLACK;
	ghost_color = (Color){
		joint_color.r,
		joint_color.g,
		joint_color.b,
		55,
	};
};

void Player::create(dWorldID world, dSpaceID space) {
	for (auto& [body_name, b] : body) {
		b.create(world, space);
	}

	for (auto& [joint_name, j] : joint) {
		//j.create_joint(world, space, mass, body[j.connections[0]], body[j.connections[1]]);
	}
};

void Player::update_freeze() {
	for (auto& [body_name, b] : body) {
		b.update_freeze();
	}

	for (auto& [joint_name, j] : joint) {
		//j.update_joint_freeze();
	}
};

void Player::ReFreeze() {
	for (auto& [body_name, b] : body) {
		b.ReFreeze();
	}

	for (auto& [joint_name, j] : joint) {
		//j.refreeze_joint();
	}
};

void Player::draw() {
	for (auto& [body_name, b] : body) {
		b.draw();
	}

	for (auto& [joint_name, j] : joint) {
		//j.draw_joint();
	}
};

void Player::draw_freeze() {
	for (auto& [body_name, b] : body) {
		b.draw_freeze();
	}

	for (auto& [joint_name, j] : joint) {
		//j.draw_joint_freeze();
	}
};

void Player::draw_ghost() {
	if (ghost) {
		for (auto& [body_name, b] : body) {
			b.draw_ghost();
		}

		for (auto& [joint_name, j] : joint) {
			//j.draw_joint_ghost();
		}
	}
};

void Player::toggle_ghost() {
	for (auto& [body_name, b] : body) {
		b.toggle_ghost();
	}

	for (auto& [joint_name, j] : joint) {
		//j.toggle_ghost();
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
	ReFreeze();
	if (passive_states == RELAX_ALL) {
		TriggerPlayerPassiveStates(HOLD_ALL);
		passive_states = HOLD_ALL;
	} else {
		TriggerPlayerPassiveStates(RELAX_ALL);
		passive_states = RELAX_ALL;
	}
}

void Player::TogglePlayerPassiveStatesAlt() {
	ReFreeze();
	if (passive_states_alt == RELAX_ALL) {
		TriggerPlayerPassiveStatesAlt(HOLD_ALL);
		passive_states_alt = HOLD_ALL;
	} else {
		TriggerPlayerPassiveStatesAlt(RELAX_ALL);
		passive_states_alt = RELAX_ALL;
	}
}

void Player::RelaxAll() {
	ReFreeze();
	TriggerPlayerPassiveStates(RELAX_ALL);
	passive_states = RELAX_ALL;
};

void Player::RelaxAllAlt() {
	ReFreeze();
	TriggerPlayerPassiveStatesAlt(RELAX_ALL);
	passive_states_alt = RELAX_ALL;
};
