#include "game.h"
#include "camera.h"
#include "replay.h"
#include "luau.h"
#include "api.h"

using namespace raylib;
#include "raymath.h"
#include "rlgl.h"

bool Game::Running ()
{
	return state.running;
}

void Game::Init()
{
	dInitODE();
	world = dWorldCreate();

	step = 1.0E-2;

	API::Init();
	API::loadscript("init");

	state.time = GetTime();
	state.running = false;

	SetExitKey(0);
}

void Game::NewGame()
{
	if (space != nullptr) {
		dSpaceDestroy(space);
	}

	if (contactgroup != nullptr) {
		dJointGroupDestroy(contactgroup);
	}

	if (objects.size() > 0) {
		objects.clear();
	}

	if (players.size() > 0) {
		players.clear();
	}

	rules = API::GetRules();
	rules.max_contacts = 8;

	o_count = API::GetObjectsCount();
	p_count = API::GetPlayersCount();

	objects.reserve(o_count);
	players.reserve(p_count);

	objects = API::GetObjects();
	players = API::GetPlayers();

	state.game_frame = 0;

	state.freeze = true;
	state.freeze_time = GetTime();
	state.freeze_frames = 50;
	state.freeze_count = 0;

	state.step_frames = 0;
	state.step_count = 0;

	space = dHashSpaceCreate(0);
  	contactgroup = dJointGroupCreate(0);
	floor = dCreatePlane(space, 0, 0, 1, 0);

	dGeomSetCategoryBits(floor, 0b0001);
	dGeomSetCollideBits(floor, 0b0000);

  	dWorldSetGravity(world, rules.gravity.x, rules.gravity.y, rules.gravity.z);

	for (auto& o : objects) {
		o.Create(world, space);
	}

	Color colors[] = {
		MAROON,
		DARKBLUE,
		DARKGREEN,
		DARKPURPLE,
	};

	uint32_t cat_bits[][2] = {
		{0b000000000010, 0b000000000100},
		{0b000000001000, 0b000000010000},
		{0b000000100000, 0b000001000000},
		{0b000010000000, 0b000100000000},
	};

	uint32_t col_bits[][2] = {
		{
			0b0001|cat_bits[1][0]|cat_bits[2][0]|cat_bits[3][0],
			0b0001|cat_bits[1][1]|cat_bits[2][1]|cat_bits[3][1],
		},
		{
			0b0001|cat_bits[0][0]|cat_bits[2][0]|cat_bits[3][0],
			0b0001|cat_bits[0][1]|cat_bits[2][1]|cat_bits[3][1],
		},
		{
			0b0001|cat_bits[0][0]|cat_bits[1][0]|cat_bits[3][0],
			0b0001|cat_bits[0][1]|cat_bits[1][1]|cat_bits[3][1],
		},
		{
			0b0001|cat_bits[0][0]|cat_bits[1][0]|cat_bits[2][0],
			0b0001|cat_bits[0][1]|cat_bits[1][1]|cat_bits[2][1],
		},
	};

	for (auto& p : players) {
		auto id = p.GetID();

		p.b_count = p.body.size();
		p.j_count = p.joint.size();
		
		if (rules.engageheight || rules.engagedistance) {
			p.SetOffset();
		}

		if (rules.engageheight) {
			p.SetEngageheight(rules.engageheight);
		}

		if (rules.engagedistance) {
			p.SetEngagedistance(rules.engagedistance,  id * (360/rules.numplayers));
		}

		p.SetColors(RAYWHITE, colors[id], Fade(colors[id], 0.10));
		p.SetCatBits(cat_bits[id][0], cat_bits[id][1]);
		p.SetColBits(col_bits[id][0], col_bits[id][1]);
		p.Create(world, space);

		if (rules.engageheight || rules.engagedistance) {
			p.SetOffset();
		}
	}


	state.running = true;

	Replay::RecordFrame();

	API::NewGameCallback();
}

void Game::Quit()
{
	API::Close();
	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(space);
	dCloseODE();

	if (Window::Initialized()) {
		Window::Close();
	}
}

void Game::NearCallback(dGeomID o1, dGeomID o2)
{
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	/* if geoms are static (immovable) don't collide */
	/* if geoms share the same body dont collide */
	/* if geoms share the same joint dont collide */

	if (0 == b1 || 0 == b2 && b1 == b2 && dAreConnected(b1, b2)) {
		return;
	}

	/*
	uint32_t cat1 = dGeomGetCategoryBits(o1);
	uint32_t col1 = dGeomGetCollideBits(o1);

	uint32_t cat2 = dGeomGetCategoryBits(o2);
	uint32_t col2 = dGeomGetCollideBits(o2);

 	if (!((cat1 & col2) || (cat2 & col1))) {
		return;
	}*/


	dContact contacts[rules.max_contacts];

	for (int i = 0; i < rules.max_contacts; i += 1) {
		contacts[i].surface = (dSurfaceParameters) {
			.mode = dContactApprox1,
			.mu = rules.friction,
		};

		m_frame_contacts[i] = contacts[i];
	}

	if (int numc = dCollide(o1, o2, rules.max_contacts, &contacts->geom, sizeof(dContact))) {
		for (int i = 0; i < numc; i += 1) {
			dJointID c = dJointCreateContact(world, contactgroup, contacts + i);
			dJointAttach(c, b1, b2);
		}
	}
}

static void nearCallback(void*, dGeomID o1, dGeomID o2)
{
	Game::NearCallback(o1, o2);
	Game::has_contact = true;
}

void Game::Refreeze()
{
	state.freeze_count = 0;

	for (auto& o : objects) {
		o.Refreeze();
	}

	for (auto& p : players) {
		p.Refreeze();
	}
}

void Game::Restart()
{
	for (auto& o : objects) {
		o.Reset();
	}
	
	for (auto& p : players) {
		p.Reset();
	}
}

void Game::Reset()
{
	objects.clear();
	players.clear();

	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(space);

	NewGame();
}

void Game::EnterEvent(EventType event)
{
	state.event = event;

	switch(event)
	{
	case STEP: {
		state.freeze = false;

		Refreeze();
	} break;
	case FREEZE: {
		state.freeze = true;
		state.freeze_time = GetTime();
		state.step_count = 0;

		for (auto& o : objects) {
			o.Freeze();
		}
	
		for (auto& p : players) {
			p.Freeze();
		}
	
		for (int i = 0; i < rules.max_contacts; i += 1) {
			m_freeze_contacts[i] = m_frame_contacts[i];
		}
	} break;
	}
}

void Game::Freeze()
{
	EnterEvent(FREEZE);	
}


void Game::Step(int frame_count)
{
	switch(state.mode)
	{
	case SELF_PLAY: {
		bool ready = true;

		if (state.selected_player != -1) {
			players[state.selected_player].Ready();
		}

		for (auto& p : players) {
			if (!p.IsReady()) {
				state.selected_player = p.GetID();
				ready = false;
				break;
			}
		}
		
		if (!ready) {
			break;
		}
	};
	case FREE_PLAY: {
		state.step_frames = frame_count;
		EnterEvent(STEP);
	} break;
	}
}

void Game::Step()
{
	Game::Step(1);
}

void Game::UpdateState(dReal dt)
{
	if (!state.freeze) {
		state.game_frame += 1;
		switch (state.mode) {
		case SELF_PLAY: {};
		case FREE_PLAY: {
			state.step_count += 1;
			if (state.step_count >= state.step_frames) {
				Freeze();
			}

			Replay::RecordFrame(state.game_frame);
		} break;
		case REPLAY_PLAY: {
			auto& frames = Replay::Get();
			dReal size = frames.size();
			if (state.game_frame > size + 100) {
				EnterMode(REPLAY_PLAY);
			} else if (state.game_frame < size) {
				Replay::Play(state.game_frame);
			}
		} break;
		}
	} else {
		switch (state.mode) {
		case SELF_PLAY: {};
		case FREE_PLAY: {
			state.freeze_count += 1;
			if (state.freeze_count >= state.freeze_frames) {
				Refreeze();
			}

			if (0 < rules.reaction_time) {
				state.reaction_count = GetTime() - state.freeze_time;
				if (state.reaction_count >= rules.reaction_time) {
					Step(rules.turnframes);
				}
			}
		} break;
		}
	}
}


void Game::TriggerCallback(CallbackType type, void* arg)
{
	if (callbacks[type] != nullptr) {
		callbacks[(int)type](arg);
	}
}

void Game::SetCallback(CallbackType type, void(*callback)(void*))
{
	callbacks[(int)type] = callback;
}

void Game::Update(dReal dt)
{
	if (!state.pause) {
		UpdateState(dt);

		dSpaceCollide(space, 0, nearCallback);
		dWorldStep(world, step);
		dJointGroupEmpty(contactgroup);

		for (auto& o : objects) {
			o.Step();
		}
	
		for (auto& p : players) {
			p.Step();
		}
	}

	API::UpdateCallback(dt);
}

void Game::DrawContacts(bool freeze)
{
	dContact* contacts;

	if (freeze) {
		contacts = m_freeze_contacts;
	} else {
		contacts = m_frame_contacts;
	}

	for (int i = 0; i < rules.max_contacts; i += 1) {
		auto position = contacts[i].geom.pos;
		DrawSphere((Vector3){position[0], position[1], position[2]}, 0.10, Fade(YELLOW, 0.10));
	}
}

void Game::DrawFloor()
{
	rlPushMatrix();
	Vector3 axis;
	float angle;
	QuaternionToAxisAngle(QuaternionFromMatrix(MatrixRotateX(DEG2RAD*90)), &axis, &angle);
	rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);
	DrawPlane((Vector3){ 0 },  { 20.1f, 20.1f }, BLACK);
	DrawPlane((Vector3){ 0 },  { 20.0f, 20.0f }, WHITE);
	DrawGrid(20, 1.0f);
	rlPopMatrix();
}

void Game::Draw()
{
	for (auto& o : objects) {
		o.Draw(state.freeze);
	}

	for (auto& p : players) {
		p.Draw(state.freeze);
	}

	DrawContacts(state.freeze);
}

bool Game::GetPause()
{
	return state.pause;
}

bool Game::GetFreeze()
{
	return state.freeze;
}

Gamemode Game::GetGamemode()
{
	return state.mode;
}

Gamerules& Game::GetGamerules()
{
	return rules;
}

std::string_view Game::GetMod()
{
	return rules.mod;
}

int Game::GetGameFrame()
{
	return state.game_frame;
}

dReal Game::GetReactionTime()
{
	return rules.reaction_time;
}

dReal Game::GetReactionCount()
{
	return state.reaction_count;
}

size_t Game::GetObjectCount()
{
	return objects.size();
}

size_t Game::GetPlayerCount()
{
	return players.size();
}

size_t Game::GetPlayerBodyCount(PlayerID player_id)
{
	return players[player_id].body.size();
}

size_t Game::GetPlayerJointCount(PlayerID player_id)
{
	return players[player_id].joint.size();
}

std::vector<Body> Game::GetObjects()
{
	return objects;
}

Player& Game::GetPlayer(PlayerID player_id)
{
	return players[player_id];
}

Player& Game::GetSelectedPlayer()
{
	return players[state.selected_player];
}

PlayerID Game::GetSelectedPlayerID()
{
	return state.selected_player;
}

Joint& Game::GetJoint(PlayerID player_id, JointID joint_id)
{
	return players[player_id].joint[joint_id];
}

Joint& Game::GetSelectedJoint()
{
	return players[state.selected_player].joint[state.selected_joint];
}

JointID Game::GetSelectedJointID()
{
	return state.selected_joint;
}

std::vector<Player> Game::GetPlayers()
{
	return players;
}

double Game::GetFrameTime()
{
	return raylib::GetFrameTime();
}

double Game::GetTime()
{
	return raylib::GetTime() - state.time;
}

void Game::SetSelectedJoint()
{
	state.selected_joint = -1;
}

void Game::SetSelectedJoint(JointID joint_id)
{
	state.selected_joint = joint_id;
}

void Game::SetSelectedPlayer()
{
	state.selected_player = -1;
}

void Game::SetSelectedPlayer(PlayerID player_id)
{
	state.selected_player = player_id;
}

void Game::TogglePause()
{
	state.pause = state.pause == false;
}

void Game::ToggleGhosts()
{
	for (auto& p : players) {
		if (p.GetID() != state.selected_player && 0 > state.selected_player) {
			p.ToggleGhost();
		}
	}

	Refreeze();
}

void Game::TogglePlayerPassiveStatesAlt(PlayerID player_id)
{
	players[player_id].TogglePassiveStatesAlt();
}

void Game::TogglePlayerPassiveStates(PlayerID player_id)
{
	players[player_id].TogglePassiveStates();
}

void Game::ToggleSelectedPlayerPassiveStatesAlt()
{
	players[state.selected_player].TogglePassiveStatesAlt();
}

void Game::ToggleSelectedPlayerPassiveStates()
{
	players[state.selected_player].TogglePassiveStates();
}

void Game::ToggleJointActiveStateAlt(JointID joint_id)
{
	players[state.selected_player].joint[joint_id].ToggleActiveStateAlt();
}

void Game::ToggleJointActiveState(JointID joint_id)
{
	players[state.selected_player].joint[joint_id].ToggleActiveState();
}

void Game::ToggleJointPassiveStateAlt(JointID joint_id)
{
	players[state.selected_player].joint[joint_id].TogglePassiveStateAlt();
}

void Game::ToggleJointPassiveState(JointID joint_id)
{
	players[state.selected_player].joint[joint_id].TogglePassiveState();
}

void Game::CycleJointStateAlt(JointID joint_id)
{
	players[state.selected_player].joint[joint_id].CycleStateAlt();
}

void Game::CycleJointState(JointID joint_id)
{
	players[state.selected_player].joint[joint_id].CycleState();
}

void Game::ToggleSelectedJointActiveStateAlt()
{
	players[state.selected_player].joint[state.selected_joint].ToggleActiveStateAlt();
}

void Game::ToggleSelectedJointActiveState()
{
	players[state.selected_player].joint[state.selected_joint].ToggleActiveState();
}

void Game::ToggleSelectedJointPassiveStateAlt()
{

	players[state.selected_player].joint[state.selected_joint].TogglePassiveStateAlt();
}

void Game::ToggleSelectedJointPassiveState()
{
	players[state.selected_player].joint[state.selected_joint].TogglePassiveState();
}

void Game::CycleSelectedJointStateAlt()
{
	players[state.selected_player].joint[state.selected_joint].CycleStateAlt();
}

void Game::CycleSelectedJointState()
{
	players[state.selected_player].joint[state.selected_joint].CycleState();
}

void Window::Init()
{
	SetTraceLogLevel(LOG_ERROR);
	InitWindow(width, height, "TOBAS");

	background = LoadRenderTexture(width, height);
	foreground = LoadRenderTexture(width, height);

	initialized = true;

	Gamecam::Init();
}

static Ray MouseRay = { 0 };
static RayCollision MouseCollision = { 0 };

static void SelectPlayer(Camera3D camera)
{
	using namespace Game;
	RayCollision collision = { 0 };
	MouseRay = GetMouseRay(GetMousePosition(), camera);

	for (auto& o : objects) {
		MouseCollision = o.collide_mouse_ray(MouseRay, MouseCollision);
		if (MouseCollision.hit) {
			collision = MouseCollision;
			SetSelectedPlayer();
			break;
		}
	}

	for (auto& p : players) {
		for (auto& b : p.body) {
			MouseCollision = b.collide_mouse_ray(MouseRay, MouseCollision);
			if (MouseCollision.hit) {
				collision = MouseCollision;
				SetSelectedPlayer(p.GetID());
				break;
			}
		}

		for (auto& j : p.joint) {
			MouseCollision = j.collide_mouse_ray(MouseRay, MouseCollision);
			if (MouseCollision.hit) {
				collision = MouseCollision;
				SetSelectedPlayer(p.GetID());
				break;
			}
		}
	}

	if (!collision.hit) {
		SetSelectedPlayer();
	}
}

static void SelectBody(Camera3D camera)
{
	using namespace Game;
	RayCollision collision = { 0 };
	MouseRay = GetMouseRay(GetMousePosition(), camera);
	for (auto& b : players[state.selected_player].body) {
		MouseCollision = b.collide_mouse_ray(MouseRay, MouseCollision);
		if (MouseCollision.hit) {
			collision = MouseCollision;
			if (b.interactive) {
				b.active = b.active == false;
			}
			break;
		}
	}
}

static void SelectJoint(Camera3D camera)
{
	using namespace Game;
	RayCollision collision = { 0 };
	MouseRay = GetMouseRay(GetMousePosition(), camera);
	for (auto& j : players[state.selected_player].joint) {
		MouseCollision = j.collide_mouse_ray(MouseRay, MouseCollision);
		if (MouseCollision.hit) {
			collision = MouseCollision;
			SetSelectedJoint(j.GetID());
			j.select = true;
			break;
		} else {
			SetSelectedJoint();
			j.select = false;
		}
	}
}

void Window::Update()
{
	SetWindowTitle(TextFormat("TOBAS %dFPS", GetFPS()));

	const auto& camera = Gamecam::Get();

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && Game::GetSelectedPlayerID() >= 0) {
		SelectBody(camera);
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		SelectPlayer(camera);
	}

	if (Game::GetSelectedPlayerID() >= 0) {
		SelectJoint(camera);
	}


	if (0 > Game::GetSelectedPlayerID()) {
		Gamecam::UpdateSpectatorcam(Game::GetFreeze(), Game::GetPlayers());
	} else {
		Gamecam::UpdatePlaycam(Game::GetFreeze(), Game::GetSelectedPlayer());
	}

	Gamecam::Update();

	if (IsFileDropped()) {
		FilePathList dropped_files = LoadDroppedFiles();
		UnloadDroppedFiles(dropped_files);
	}
}

void Window::RenderBackground(Camera3D camera)
{
	BeginTextureMode(background);
		ClearBackground(RAYWHITE);
		BeginMode3D(camera);
			Game::Draw();
		EndMode3D();
	EndTextureMode();
}

void Window::RenderForeground(Camera3D camera)
{
	BeginTextureMode(foreground);
		BeginMode3D(camera);
		ClearBackground(Fade(WHITE, 0.00));
		EndMode3D();
	EndTextureMode();
}

void Window::SetDrawCallback(void (*callback)(float, float))
{
	DrawCallback = callback;
}

void Window::Draw()
{
	const auto& camera = Gamecam::Get();

	RenderBackground(camera);
	RenderForeground(camera);
	BeginDrawing();
	DrawTextureRec(background.texture, {0, 0, width, -height}, {0, 0}, WHITE);
	DrawTextureRec(foreground.texture, {0, 0, width, -height}, {0, 0}, WHITE);	

	API::DrawCallback();

	EndDrawing();
}

void Window::Close()
{
	UnloadRenderTexture(background);
	UnloadRenderTexture(foreground);
	CloseWindow();
}

bool Window::Initialized()
{
	return initialized;
}

float Window::GetWidth()
{
	return width;
}

float Window::GetHeight()
{
	return height;
}

void Game::EnterMode(Gamemode mode)
{
	state.mode = mode;
	
	for (auto& p : players) {
		p.RelaxAll();
		p.RelaxAllAlt();
	}

	state.game_frame = 0;
	state.reaction_count = 0;
	state.freeze_count = 0;
	state.step_count = 0;
	
	switch(mode)
	{
	case FREE_PLAY: {
		state.freeze = true;

		Restart();
	
		Freeze();
	
		Replay::RecordFrame();
	} break;
	case SELF_PLAY: {
		state.freeze = true;

		Restart();
	
		Freeze();
	
		Replay::RecordFrame();
	} break;
	case REPLAY_PLAY: {
		state.freeze = false;
		
		Restart();
	} break;
	case REPLAY_EDIT: {
		state.freeze = true;

		Restart();
	
		Freeze();
	} break;
	}
}
