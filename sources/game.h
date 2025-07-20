#pragma once
#include "common.h"
#include "player.h"

enum Gamemode
{
	FREE_PLAY,
	SELF_PLAY,
	NET_PLAY,
	REPLAY_PLAY,
	REPLAY_EDIT,
};

enum EventType
{
	STEP,
	FREEZE,
};

enum CallbackType
{
	NEWGAME,
	UPDATE,
};

typedef void (*Callback)(void*);

struct Gamerules {
	std::string_view mod;

	int numplayers;
	int turnframes;
	int max_contacts;
	dReal reaction_time;
	dReal engagedistance;
	dReal engageheight;
	dReal friction;

	raylib::Vector3 gravity;
};

struct Gamestate {
	Gamemode mode = FREE_PLAY;
	EventType event = FREEZE;

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


namespace Game {
	static dWorldID world;
	static dSpaceID space;
	static dJointGroupID contactgroup;
	
	static dGeomID floor;
		
	static Gamestate state;
	static Gamerules rules;
	
	static std::vector<Body> objects;

	static std::vector<Body> dynamic_objects;
	static std::vector<Body> static_objects;
	static std::vector<Joint> joint_objects;

	static std::vector<Player> players;

	static size_t o_count;
	static size_t p_count;
	
	static dMass mass;

	static dReal step;

	static dContact m_frame_contacts[1024];
	static dContact m_freeze_contacts[1024];
	static bool has_contact = false;

	void Init();
	void Quit();

	void TogglePause();
	bool GetFreeze();
	bool GetPause();
	double GetTime();
	double GetFrameTime();

	void SetSelectedPlayer();
	void SetSelectedPlayer(PlayerID);

	PlayerID GetSelectedPlayerID();
	Player& GetSelectedPlayer();
	Player& GetPlayer(PlayerID player_id);

	void SetSelectedJoint();
	void SetSelectedJoint(JointID joint_id);
	JointID GetSelectedJointID();
	Joint& GetSelectedJoint();
	Joint& GetJoint(PlayerID player_id, JointID joint_id);

	Gamemode GetGamemode();
	Gamerules& GetGamerules();

	std::vector<Player> GetPlayers();
	std::vector<Body> GetObjects();
	
	static std::map<int, Callback> callbacks;

	void SetCallback(CallbackType type ,Callback callback);
	void TriggerCallback(CallbackType type, void* arg);

	void NearCallback(dGeomID o1, dGeomID o2);

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

	void UpdateState(dReal dt);
	void Update(dReal dt);

	void DrawContacts(bool freeze);
	void DrawFloor();
	void Draw();

	void EnterEvent(EventType event);
	void EnterMode(Gamemode mode);

	void Step(int);
	void Step();
	void Freeze();
	void Refreeze();

	void Restart();
	void Loop();

	bool Running();
};

namespace Window
{
	static bool initialized = false; 

	static float  width = 800;
	static float height = 450;

	static raylib::RenderTexture background;
	static raylib::RenderTexture foreground;

	static void (*DrawCallback)(float, float) = nullptr;

	void Init();
	void Update(raylib::Camera3D);

	void SetDrawCallback(void (*callback)(float, float));

	void RenderBackground(raylib::Camera3D camera);
	void RenderForeground(raylib::Camera3D camera);
	void Draw(raylib::Camera3D camera);

	void  Close();
	bool  Initialized();
	float GetWidth ();
	float GetHeight();
};
