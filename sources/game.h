#pragma once
#include "common.h"
#include "player.h"

enum Gamemode {
	FREEPLAY,
	REPLAY,
};

struct Gamerules {
	std::string_view mod;

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
	Gamemode mode = FREEPLAY;

	double time = 0;

	int game_frame = 0;

	dReal reaction_count = 0;

	dReal freeze_time = 0;
	int freeze_frames = 0;
	int freeze_count = 0;

	dReal step_time = 0;
	int step_frames = 0;
	int step_count = 0;

	PlayerID selected_player = -1;
	JointID  selected_joint  = -1;

	bool running = false;
	bool freeze = false;
	bool pause = false;
};

struct CollisionData {
	dContact contacts;
};

namespace Game {
	static dWorldID world;
	static dSpaceID space;
	static dJointGroupID contactgroup;
	
	static dGeomID floor;
		
	static Gamestate state;
	static Gamerules rules;
	
	static std::vector<Body> objects;
	static std::vector<Player> players;
	
	static dMass mass;

	static dReal step;

	static CollisionData collision;

	static dNearCallback* nearCallback;

	void Init();
	void Quit();

	void TogglePause();
	bool GetFreeze();
	bool GetPause();
	double GetTime();

	void SetSelectedPlayer(PlayerID);

	const Player& GetPlayer(PlayerID player_id);
	Player GetSelectedPlayer();
	PlayerID GetSelectedPlayerID();
	Joint GetSelectedJoint();
	JointID GetSelectedJointID();

	Gamemode GetGamemode();
	Gamerules GetGamerules();
	std::vector<Player> GetPlayers();
	std::vector<Body> GetObjects();

	void NearCallback(dGeomID, dGeomID);

	std::string_view GetMod();

	size_t GetObjectCount();
	size_t GetPlayerCount();
	size_t GetPlayerBodyCount(PlayerID player_id);
	size_t GetPlayerJointCount(PlayerID player_id);

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

	void ModeFreeplay();
	void ModeReplay();

	void Restart();
	void UpdateFreeze();
	void Step(int);
	void Step();
	void Refreeze();
	void Loop();

	bool Running();
};

namespace Window
{
	static float  width = 800;
	static float height = 450;
	float GetWidth ();
	float GetHeight();
};
