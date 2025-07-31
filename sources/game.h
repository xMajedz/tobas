#pragma once
#include "common.h"
#include "player.h"

enum Gamemode
{
	FREE_PLAY,
	SELF_PLAY,
	REPLAY_PLAY,
	REPLAY_EDIT,

	FREE_PLAY_TOGETEHR,
};

enum EventType
{
	STEP,
	FREEZE,
};

struct Gamerules
{
	std::string_view mod = "nomod";

	int numplayers;
	int turnframes;
	int max_contacts;
	dReal reaction_time;
	dReal engagedistance;
	dReal engageheight;
	dReal friction;

	vec3 gravity;
};

struct Gamestate
{
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


namespace Game
{
	static dWorldID world = nullptr;
	static dSpaceID space = nullptr;
	static dJointGroupID contactgroup = nullptr;
	
	static dGeomID floor = nullptr;
		
	static Gamestate state;
	static Gamerules rules;
	
	static std::vector<Body> objects;

	static std::vector<Body> dynamic_objects;
	static std::vector<Body> static_objects;
	static std::vector<Joint> joint_objects;

	static std::vector<Player> players;

	static size_t o_count;
	static size_t jo_count;

	static size_t p_count;
	
	static dMass mass;

	static dReal step;

	static dContact m_frame_contacts[1024];
	static dContact m_freeze_contacts[1024];

	static int numcontacts;
	static int numcollisions;

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

	void TriggerPlayerJointState(PlayerID player_id, JointID joint_id, JointState state);
	void TriggerPlayerJointStateAlt(PlayerID player_id, JointID joint_id, JointState state);

	void TogglePlayerPassiveStatesAlt(PlayerID player_id);
	void TogglePlayerPassiveStates(PlayerID player_id);
	void ToggleSelectedPlayerPassiveStatesAlt();
	void ToggleSelectedPlayerPassiveStates();

	void ToggleJointActiveStateAlt(JointID selected_joint_id);
	void ToggleJointActiveState(JointID selected_joint_id);
	void ToggleJointPassiveStateAlt(JointID selected_joint_id);
	void ToggleJointPassiveState(JointID selected_joint_id);
	void CycleJointStateAlt(JointID selected_joint_id);
	void CycleJointState(JointID selected_joint_id);

	void ToggleSelectedJointActiveStateAlt();
	void ToggleSelectedJointActiveState();
	void ToggleSelectedJointPassiveStateAlt();
	void ToggleSelectedJointPassiveState();

	void CycleSelectedJointStateAlt();
	void CycleSelectedJointState();

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

	void Init();
	void Update();

	void RenderBackground(raylib::Camera3D camera);
	void RenderForeground(raylib::Camera3D camera);
	void Draw();

	void  Close();
	bool  Initialized();
	float GetWidth ();
	float GetHeight();
};
