#pragma once
#include <common.h>
#include <camera.h>
#include <player.h>

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
	bool running = false;
};

struct FrameData : public FreezeData {
	std::map<std::string, Player> player;
};

struct CollisionData {
	dContact contacts;
};

struct Game {
	dWorldID world;
	dSpaceID space;
	dJointGroupID contactgroup;

	dGeomID floor;
	dReal step = 1.0E-2;

	CollisionData collision;

	Gamecam gamecam;
	Gamestate state;
	Gamerules rules;
	std::map<std::string, Body> objects;
	std::map<std::string, Player> players;
	std::map<int, FrameData> RecordedFrames;

	void TogglePause();
	bool GetFreeze();
	bool GetPause();
	Gamerules GetGamerules();
	std::map<std::string, Player> GetPlayers();
	std::map<std::string, Body> GetObjects();

	dNearCallback* nearCallback;
	void NearCallback(dGeomID, dGeomID);

	void Start();
	void NewGame();
	void EndGame();
	void ResetGame();
	void ToggleGhosts();
	void DrawFloor();
	void DrawFrame();
	void SelectPlayer(Camera3D, Ray, RayCollision);
	void SelectJoint(Camera3D, Ray, RayCollision);
	void RecordFrame(int);
	void PlayFrame(int);
	void EditReplay();
	void SaveReplay();
	void StartFreeplay();
	void StartReplay();
	void UpdateFrame();
	void Restart();
	void UpdateFreeze();
	void StepGame(int);
	void ReFreeze();
	void Loop();

	bool Running();

	void UpdatePlaycam(Gamecam* gamecam);
};

struct Window {
	int width;
	int height;
};

struct Console {
	char* messages[1024];
	char* last_message;

	void log(lua_State* L, char* message);
};
