#ifndef GAME_H
#define GAME_H

enum Gamemode {
	FREEPLAY,
	REPLAY,
};

struct Gamerules {
	int numplayers = 2;
	int turnframes = 10;
	int max_contacts = 8;
	dReal reaction_time = 10;
	dReal engagedistance;
	dReal engageheight;
	dReal gravity[3];
	dReal friction;
};

struct Gamestate {
	Gamemode gamemode = FREEPLAY;

	bool freeze = false;
	bool pause = false;

	int game_frame;

	dReal reaction_count;

	dReal freeze_time;
	int freeze_frames;
	int freeze_count;

	dReal step_time;
	int step_frames;
	int step_count;

	std::string selected_player = "NONE";
	std::string selected_joint = "NONE";
};

class Game {
	public:
	dWorldID world;
	dSpaceID space;
	dJointGroupID contactgroup;
	dGeomID floor;
	dReal step = 1.0E-2;

	Gamestate state;
	Gamerules rules;
	std::map<std::string, Body> objects;
	std::map<std::string, Player> players;

	void TogglePause();
	bool GetFreeze();
	bool GetPause();
	Gamerules GetGamerules();
	std::map<std::string, Player> GetPlayers();
	std::map<std::string, Body> GetObjects();
} game;
#endif
