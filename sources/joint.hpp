#ifndef JOINT_H
#define JOINT_H

enum JointType {
	Hinge,
	Slider,
	Universal,
	Hinge2,
};

enum JointState {
	RELAX,
	HOLD,
	FORWARD,
	BACKWARD,
};

class Joint : public Body {
	public:
	std::string connections[2];
	JointType connectionType;
	JointState state;
	JointState state_alt;
	dJointID dJoint;
	dReal axis[3];
	dReal axis_alt[3];
	dReal range[2];
	dReal range_alt[2];
	dReal strength;
	dReal strength_alt;
	dReal velocity;
	dReal velocity_alt;

	void refreeze_joint();
	
	void update_joint_freeze();
	
	void create_joint(dMass mass, Body b1, Body b2);
	
	void draw_joint(Color draw_color);
	
	void draw_joint_freeze();
	
	void draw_joint_ghost();
	
	void draw_joint();
	
	void TriggerActiveStateAlt(dReal direction);
	
	void TriggerPassiveStateAlt(dReal strength);
	
	void TriggerActiveState(dReal direction);
	
	void TriggerPassiveState(dReal strength);
	
	void TogglePassiveState();
	
	void TogglePassiveStateAlt();
	
	void ToggleActiveState();
	
	void ToggleActiveStateAlt();
	
	void CycleState();
	
	void CycleStateAlt();
};
#endif
