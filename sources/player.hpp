#ifndef PLAYER_H
#define PLAYER_H

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
	
	dReal engagepos[3];
	dReal engagerot[3];

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
	
	void create();
	
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
#endif
