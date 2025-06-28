#include <common.h>
#include <body_class.h>
#include <joint_class.h>

enum PlayerPassiveStates {
	HOLD_ALL,
	RELAX_ALL,
	MIXED,
}; 

class Player {
	public:
	std::string name;
	std::map<std::string, Body> body;
	std::map<std::string, Joint> joint;
	PlayerPassiveStates passive_states;
	PlayerPassiveStates passive_states_alt;
	
	Vector3 engagepos;
	Vector3 engagerot;

	bool use_engagepos;

	unsigned long body_category_bits;
	unsigned long body_collide_bits;
	unsigned long joint_category_bits;
	unsigned long joint_collide_bits;

	bool ghost;
	Color ghost_color;
	Color joint_color;
	Color joint_select_color;
	Color body_color;
	
	dMass mass;

	Player();

	void create(dWorldID world, dSpaceID space);
	
	void update_freeze();
	
	void ReFreeze();
	
	void draw();
	
	void draw_freeze();
	
	void draw_ghost();
	
	void toggle_ghost();
	
	void TriggerPlayerPassiveStates(PlayerPassiveStates state);
	
	void TriggerPlayerPassiveStatesAlt(PlayerPassiveStates state);
	
	void TogglePlayerPassiveStates();
	
	void TogglePlayerPassiveStatesAlt();
	
	void RelaxAll();
	
	void RelaxAllAlt();
};
