#pragma once
#include "common.h"
#include "player.h"

enum Gamemode {
	FREEPLAY,
	REPLAY,
};

struct Gamerules {
	std::string mod;
	int numplayers;
	int turnframes;
	int max_contacts;
	dReal reaction_time;
	dReal engagedistance;
	dReal engageheight;
	dReal friction;

	Vector3 gravity;
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
	bool running = false;
};

struct CollisionData {
	dContact contacts;
};

namespace Game {
	static dWorldID s_world;
	static dSpaceID s_space;
	static dJointGroupID s_contactgroup;
	
	static dGeomID s_floor;
		
	static Gamestate s_state;
	static Gamerules s_rules;
	
	static array<Body> s_objects;
	static array<Player> s_players;
	
	static dReal s_step;

	static CollisionData s_collision;

	static dNearCallback* s_nearCallback;

	void Init();
	void Quit();

	void TogglePause();
	bool GetFreeze();
	bool GetPause();
	Gamerules GetGamerules();

	array<Player> GetPlayers();
	array<Body> GetObjects();

	void NearCallback(dGeomID, dGeomID);

	const char* GetMod();
	int GetGameFrame();
	dReal GetReactionTime();
	dReal GetReactionCount();

	void Start();
	void NewGame();
	void Reset();
	void ToggleGhosts();

	void Update(dReal dt);
	void DrawFloor();
	void Draw();

	void SelectPlayer(Camera3D, Ray, RayCollision);
	void SelectJoint(Camera3D, Ray, RayCollision);
	void PlayFrame(int);
	void EditReplay();
	void SaveReplay();
	void StartFreeplay();
	void StartReplay();
	void Restart();
	void UpdateFreeze();
	void StepGame(int);
	void ReFreeze();
	void Loop();

	bool Running();
};

struct Window {
	int width;
	int height;
};
