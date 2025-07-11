#include "common.h"
#include "body.h"
#include "joint.h"

typedef int PlayerID;

enum PlayerPassiveStates {
	HOLD_ALL,
	RELAX_ALL,
	MIXED,
}; 

struct Player {
	std::string_view m_name;
	
	std::vector<Body> body;
	std::vector<Joint> joint;

	size_t b_count;
	size_t j_count;

	PlayerPassiveStates passive_states;
	PlayerPassiveStates passive_states_alt;
	
	Vector3 m_offset;
	Vector3 engagepos;
	Vector3 engagerot;

	bool use_engagepos;
	bool use_engagerot;

	unsigned long body_category_bits;
	unsigned long body_collide_bits;
	unsigned long joint_category_bits;
	unsigned long joint_collide_bits;

	bool ghost;

	Color ghost_color;
	Color body_color;
	Color joint_color;
	Color joint_select_color;
	
	dMass mass;

	Player(std::string_view name);
	Player();

	std::string_view GetName();
	JointState GetJointState(JointID joint_id);
	JointState GetJointStateAlt(JointID joint_id);

	void create(dWorldID world, dSpaceID space);

	void set_offset();
	void set_offset(Vector3 offset);
	void set_engageheight(float offset);
	void set_engagedistance(float offset, float angle);
	void set_category_bits(unsigned long b_bits, unsigned long j_bits);
	void set_collide_bits(unsigned long b_bits, unsigned long j_bits);
	void update_freeze();
	void refreeze();
	void reset();
	void draw(bool freeze);
	void toggle_ghost();
	void TriggerPlayerPassiveStates(PlayerPassiveStates state);
	void TriggerPlayerPassiveStatesAlt(PlayerPassiveStates state);
	void TogglePlayerPassiveStates();
	void TogglePlayerPassiveStatesAlt();
	void RelaxAll();
	void RelaxAllAlt();
};
