#include "game.h"
#include "camera.h"
#include "luau.h"
#include "api.h"

#include <fstream>

using namespace raylib;
#include "raymath.h"
#include "rlgl.h"

void Game::Init()
{
	dInitODE();
	world = dWorldCreate();
	dWorldSetERP(world, 0.45);
	dWorldSetCFM(world, 10E-2);

	step = 1.0E-2;

	API::Init();
	API::loadscript("init");

	Replay::Init();

	state.time = GetTime();
	state.running = false;

	SetExitKey(KEY_NULL);
}

void Game::NewGame()
{
	if (space != nullptr)
		dSpaceDestroy(space);
	if (contactgroup != nullptr)
		dJointGroupDestroy(contactgroup);
	if (objects.size() > 0)
		objects.clear();
	if (joint_objects.size() > 0)
		joint_objects.clear();
	if (players.size() > 0)
		players.clear();

	rules = API::GetRules();
	rules.max_contacts = 8;

	o_count = API::GetObjectsCount();
	jo_count = API::GetJointObjectsCount();

	p_count = API::GetPlayersCount();

	objects.reserve(o_count);
	joint_objects.reserve(jo_count);

	players.reserve(p_count);

	objects = API::GetObjects();
	joint_objects = API::GetJointObjects();

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
	floor = dCreatePlane(space, 0.00, 0.00, 1.00, 0.10);

	dGeomSetCategoryBits(floor, 0b0001);
	dGeomSetCollideBits(floor, 0b0000);

  	dWorldSetGravity(world, rules.gravity.x, rules.gravity.y, rules.gravity.z);

	for (auto& o : objects)
		o.Create(world, space);

	for (auto& jo : joint_objects)
		jo.Create(world, space, objects[jo.connections[0]], objects[jo.connections[1]]);

	int count = 1;

	for (auto& p : players)
	{
		p.b_count = p.body.size();
		p.j_count = p.joint.size();
		p.SetCatBits(2<<count, 2<<count);
		p.SetColBits(255-(2<<count), 255-(2<<count));

		p.Create(world, space);

		p.SetEngagedistance(rules.engagedistance, p.GetID() * (360/rules.numplayers));
		p.SetEngageheight(rules.engageheight);
		p.SetOffset();

		count += 1;
	}

	state.running = true;
	
	Replay::WriteMetaData();
	//Replay::RecordFrame();

	API::NewGameCallback();
}

void Game::Quit()
{
	if (state.running) {
		dJointGroupDestroy(contactgroup);
		dSpaceDestroy(space);
		dWorldDestroy(world);
		dCloseODE();
	}

	API::Close();
	Replay::Close();

	if (Window::Initialized()) Window::Close();
}

static void nearCallback(void*, dGeomID o1, dGeomID o2)
{
	using namespace Game;

	if (o1 == nullptr || o2 == nullptr) return;

	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	if (dAreConnected(b1, b2)) return;
	
	uint32_t cat1 = dGeomGetCategoryBits(o1);
	uint32_t col1 = dGeomGetCollideBits(o1);
	uint32_t cat2 = dGeomGetCategoryBits(o2);
	uint32_t col2 = dGeomGetCollideBits(o2);

	if (!(cat1 & col2 || cat2 & col1)) return;

	dContact contacts[rules.max_contacts];

	for (int i = 0; i < rules.max_contacts; i += 1) {
		contacts[i].surface = (dSurfaceParameters) {
			.mode = dContactApprox1|dContactBounce,
			.mu = rules.friction,
			.bounce = rules.bounce,
		};

		m_frame_contacts[i] = contacts[i];
	}

	numcontacts = dCollide(o1, o2, rules.max_contacts, &contacts->geom, sizeof(dContact));

	for (int i = 0; i < numcontacts; i += 1) {
		dJointID c = dJointCreateContact(world, contactgroup, &contacts[i]);
		dJointAttach(c, b1, b2);
	}

	numcollisions += 1;

	BodyUserData* data1 = (BodyUserData*)dGeomGetData(o1);
	BodyUserData* data2 = (BodyUserData*)dGeomGetData(o2); 
	
	if (data1 != nullptr && data1->contact_joint == nullptr && data1->active) {
		data1->contact_joint = dJointCreateFixed(world, 0);
		dJointAttach(data1->contact_joint, b1, b2);
		dJointSetFixed(data1->contact_joint);
	}

	if (data1 != nullptr && data1->contact_joint != nullptr && !data1->active) {
		dJointDestroy(data1->contact_joint);
		data1->contact_joint = nullptr;
	}

	if (data2 != nullptr && data2->contact_joint == nullptr && data2->active) {
		data2->contact_joint = dJointCreateFixed(world, 0);
		dJointAttach(data2->contact_joint, b1, b2);
		dJointSetFixed(data2->contact_joint);
	}

	if (data2 != nullptr && data2->contact_joint != nullptr && !data2->active) {
		dJointDestroy(data2->contact_joint);
		data2->contact_joint = nullptr;
	}
}

void Game::Refreeze()
{
	state.freeze_count = 0;
	for (auto& o : objects) o.Refreeze();
	for (auto& p : players) p.Refreeze();
}

void Game::Restart()
{
	for (auto& o : objects) o.Reset();
	for (auto& p : players) p.Reset();
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
	case STEP:
		for (auto& o : objects) o.Step();
		for (auto& p : players) p.Step();
		break;
	case FREEZE:
		break;
	}
}

void Game::Freeze()
{
	state.freeze = true;
	state.freeze_time = GetTime();
	state.step_count = 0;

	for (auto& o : objects) o.Freeze();
	for (auto& p : players) p.Freeze();

	for (int i = 0; i < rules.max_contacts; i += 1) m_freeze_contacts[i] = m_frame_contacts[i];
}

void Game::Step(int frame_count)
{
	switch(state.mode)
	{
	case SELF_PLAY:
		{
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
		}
	case FREE_PLAY:
		state.step_frames = frame_count;
		state.freeze = false;
		Refreeze();
		break;
	}
}

void Game::Step()
{
	Step(1);
}

void Game::UpdateState(dReal dt)
{
}

void Game::Update(dReal dt)
{
	if (!state.running) {
		NewGame();
	}

	API::UpdateCallback(dt);

	if (!state.pause)
	{
		numcollisions = 0;

		if (!state.freeze) {
			switch (state.mode)
			{
			case SELF_PLAY: case FREE_PLAY:
				//Replay::RecordFrame(state.game_frame);
	
				if (state.step_count > state.step_frames) {
					Freeze();
				}
	
				state.step_count += 1;
	
				break;
			case REPLAY_PLAY:
				double frame_count = Replay::GetFrameCount();
				
				if (state.game_frame > frame_count + 100) {
					EnterMode(REPLAY_PLAY);
				}
	
				if (state.game_frame < frame_count) {
					Replay::Play(state.game_frame);
				}
	
				break;
			}

			//state.game_frame += 1;
		} else {
			switch (state.mode)
			{
			case SELF_PLAY: case FREE_PLAY:
				if (state.freeze_count >= state.freeze_frames) {
					Refreeze();
				}
	
				if (0 < rules.reaction_time) {
					state.reaction_count = GetTime() - state.freeze_time;
					if (state.reaction_count >= rules.reaction_time) Step(rules.turnframes);
				}
	
				break;
			}
	
			state.freeze_count += 1;
		}

		EnterEvent(STEP);

		dSpaceCollide(space, 0, nearCallback);
		dWorldStep(world, step);
		dJointGroupEmpty(contactgroup);
		
		if (!state.freeze) {
			switch (state.mode)
			{
			case SELF_PLAY: case FREE_PLAY:
				Replay::RecordFrame(state.game_frame);
				break;
			case REPLAY_PLAY:
				break;
			}

			state.game_frame += 1;
		} else {
			switch (state.mode)
			{
			case SELF_PLAY: case FREE_PLAY:
				break;
			}
	
		}
	}
}

void Game::DrawContacts(bool freeze)
{
	dContact* contacts;

	if (freeze) {
		contacts = m_freeze_contacts;
	} else {
		contacts = m_frame_contacts;
	}

	for (int i = 0; numcollisions != 0; i += 1) {
		auto position = contacts[i].geom.pos;
		DrawSphere((Vector3){position[0], position[1], position[2]}, 0.10, Fade(YELLOW, 0.10));
		numcollisions -= 1;
	}
}

void Game::DrawFloor()
{
	float angle;
	Vector3 axis;
	QuaternionToAxisAngle(QuaternionFromMatrix(MatrixRotateX(DEG2RAD*90)), &axis, &angle);

	rlPushMatrix();
	rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);
	DrawGrid(2, 20);
	rlPopMatrix();
}

void Game::Draw()
{
	for (auto& o : objects) o.Draw(state.freeze);
	for (auto& p : players) p.Draw(state.freeze);

	if (state.freeze && state.selected_player != -1 && state.selected_joint != -1)
		players[state.selected_player].joint[state.selected_joint].DrawSelect();

	DrawFloor();
	DrawContacts(state.freeze);
}

void Game::SetGravity(dReal x, dReal y, dReal z)
{

  	dWorldSetGravity(world, x, y, z);
	//rules.gravity = {x, y , z};
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

dReal Game::GetSelectedJointVelocity()
{
	return players[state.selected_player].joint[state.selected_joint].velocity;
}

dReal Game::GetSelectedJointVelocityAlt()
{
	return players[state.selected_player].joint[state.selected_joint].velocity_alt;
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

void Game::SetBodyState(PlayerID player_id, BodyID body_id, bool state)
{
	players[player_id].body[body_id].active = state;
}

void Game::TogglePause()
{
	state.pause = state.pause == false;
}

void Game::ToggleGhosts()
{
	for (auto& p : players) if (p.GetID() != state.selected_player && 0 > state.selected_player) p.ToggleGhost();
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

void Game::TriggerPlayerJointState(PlayerID player_id, JointID joint_id, JointState state)
{
	switch(state)
	{
	case RELAX:
		players[player_id].joint[joint_id].TriggerPassiveState(0.00);
		break;
	case HOLD:
		players[player_id].joint[joint_id].TriggerPassiveState(players[player_id].joint[joint_id].strength);
		break;
	case FORWARD:
		players[player_id].joint[joint_id].TriggerActiveState(1.00 * players[player_id].joint[joint_id].velocity);
		break; 
	case BACKWARD:
		players[player_id].joint[joint_id].TriggerActiveState(-1.00 * players[player_id].joint[joint_id].velocity);
		break;
	}
}

void Game::TriggerPlayerJointStateAlt(PlayerID player_id, JointID joint_id, JointState state)
{
	switch(state)
	{
	case RELAX:
		players[player_id].joint[joint_id].TriggerPassiveStateAlt(0.00);
		break;
	case HOLD:
		players[player_id].joint[joint_id].TriggerPassiveStateAlt(players[player_id].joint[joint_id].strength_alt);
		break;
	case FORWARD:
		players[player_id].joint[joint_id].TriggerActiveStateAlt(1.00 * players[player_id].joint[joint_id].velocity_alt);
		break; 
	case BACKWARD:
		players[player_id].joint[joint_id].TriggerActiveStateAlt(-1.00 * players[player_id].joint[joint_id].velocity_alt);
		break;
	}
}

void Game::TriggerPlayerJoint(PlayerID player_id, JointID joint_id, JointState state, dReal vel)
{
	switch(state)
	{
	case RELAX:
		players[player_id].joint[joint_id].TriggerPassiveState(0.00);
		break;
	case HOLD:
		players[player_id].joint[joint_id].TriggerPassiveState(players[player_id].joint[joint_id].strength);
		break;
	case FORWARD: case BACKWARD:
		players[player_id].joint[joint_id].TriggerActiveState(vel);
		break;
	}
}

void Game::TriggerPlayerJointAlt(PlayerID player_id, JointID joint_id, JointState state, dReal vel)
{
	switch(state)
	{
	case RELAX:
		players[player_id].joint[joint_id].TriggerPassiveStateAlt(0.00);
		break;
	case HOLD:
		players[player_id].joint[joint_id].TriggerPassiveStateAlt(players[player_id].joint[joint_id].strength_alt);
		break;
	case FORWARD: case BACKWARD:
		players[player_id].joint[joint_id].TriggerActiveStateAlt(vel);
		break;
	}
}

void Game::UndoSelectedPlayerMove()
{
	Replay::Play(state.game_frame);
}

void Game::ToggleBodyState(BodyID body_id)
{
	players[state.selected_player].body[body_id].ToggleState();
}

void Game::ToggleSelectedBodyState()
{
	players[state.selected_player].body[state.selected_body].ToggleState();
}

void Game::ToggleSelectedPlayerBodyStates()
{
	for (auto& b : players[state.selected_player].body)
	{
		if (b.m_interactive)
			b.ToggleState();
	}
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

void Game::TriggerSelectedJointActiveStateAlt(dReal vel)
{
	players[state.selected_player].joint[state.selected_joint].TriggerActiveStateAlt(vel);
}

void Game::TriggerSelectedJointActiveState(dReal vel)
{
	players[state.selected_player].joint[state.selected_joint].TriggerActiveState(vel);
}

void Game::ToggleSelectedJointActiveStateAlt(dReal vel)
{
	players[state.selected_player].joint[state.selected_joint].ToggleActiveStateAlt(vel);
}

void Game::ToggleSelectedJointActiveState(dReal vel)
{
	players[state.selected_player].joint[state.selected_joint].ToggleActiveState(vel);
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

static Shader shader;

void rl_log(int level, const char* msg, va_list)
{
	Console::log(TextFormat("%d: %s", level, msg));
}

void Window::Init()
{
	SetTraceLogLevel(LOG_ERROR);
	SetTraceLogCallback(rl_log);

	InitWindow(width, height, "TOBAS");

	Gamecam::Init();

	shader = LoadShader(NULL, "resources/shader/tobas.fs");

	background = LoadRenderTexture(width, height);
	foreground = LoadRenderTexture(width, height);

	initialized = true;

	SetTargetFPS(60);
}

static Ray MouseRay = { 0 };
static RayCollision MouseCollision = { 0 };

static int selected_object = -1;
static int selected_player = -1;
static int selected_joint = -1;
static int selected_body = -1;

static void gSelector(Camera3D camera)
{
	using namespace Game;
	RayCollision collision = { 0 };
	MouseRay = GetMouseRay(GetMousePosition(), camera);
	
	for (auto& o : objects) {
		collision = o.CollideMouseRay(MouseRay, collision);
		if (collision.hit) {
			selected_object = o.GetID();
			break;
		}
	}

	bool hit = false;
	for (auto& p : players) {
		for (auto& j : p.joint) {
			collision = j.CollideMouseRay(MouseRay, collision);
			if (collision.hit) {
				selected_player = p.GetID();
				selected_joint = j.GetID();
				hit = true;
				break;
			}
		}

		if (hit) break;

		selected_player = -1;
		selected_joint = -1;

		for (auto& b : p.body) {
			collision = b.CollideMouseRay(MouseRay, collision);
			if (collision.hit) {
				selected_player = p.GetID();
				selected_body = b.GetID();
				hit = true;
				break;
			}
		}

		if (hit) break;

		selected_player = -1;
		selected_body = -1;
	}
}

void Window::Update()
{
	if (IsWindowResized()) {
		
		width = GetScreenWidth();
		height = GetScreenHeight();

		UnloadRenderTexture(background);
		background = LoadRenderTexture(width, height);

		UnloadRenderTexture(foreground);
		foreground = LoadRenderTexture(width, height);
	}

	SetWindowTitle(TextFormat("TOBAS %dFPS", GetFPS()));

	const auto& camera = Gamecam::Get();

	gSelector(camera);

	if (Game::GetSelectedPlayerID() != -1)
		Game::SetSelectedJoint(selected_joint);
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		Game::SetSelectedPlayer(selected_player);

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

void Window::Draw()
{
	const auto& camera = Gamecam::Get();

	//RenderBackground(camera);
	//RenderForeground(camera);

	BeginDrawing();

	ClearBackground(RAYWHITE);

	BeginShaderMode(shader);

	DrawTextureRec(background.texture, {0, 0, width, -height}, {0, 0}, WHITE);
	DrawTextureRec(foreground.texture, {0, 0, width, -height}, {0, 0}, WHITE);	
	
	RenderBackground(camera);
	RenderForeground(camera);

	EndShaderMode();
	API::DrawCallback();

	EndDrawing();
}

void Window::Close()
{
	UnloadShader(shader);
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
	auto prev_mode = FREE_PLAY;

	if (mode == REPLAY_EDIT) {
		prev_mode = mode;
	} else {
		state.game_frame = 0;
		Restart();
	}

	for (auto& p : players) {
		p.RelaxAll();
		p.RelaxAllAlt();

		for (auto& b : p.body) {
			b.active = false;
		}
	}

	state.reaction_count = 0;
	state.freeze_count = 0;
	state.step_count = 0;
	
	switch(mode)
	{
	case REPLAY_PLAY:
		state.mode = mode;
		state.freeze = false;
		break;
	case REPLAY_EDIT: case SELF_PLAY: case FREE_PLAY:
		if (mode == REPLAY_EDIT) {
			mode = prev_mode;
		}

		state.mode = mode;

		Freeze();

		Replay::Begin();

		break;
	}
}

bool Game::Running ()
{
	return state.running;
}

void Replay::Init()
{
	storage = new Arena(8*1024*1024);
}

void Replay::Close()
{
	delete storage;
}

void Replay::Destroy()
{
	frame_count = 0;
	storage->clear();

	frames = storage->allocate(sizeof(FrameData)*4096);
}

void Replay::WriteMetaData()
{
	auto mod = Game::GetMod();
	auto p_count = Game::GetPlayerCount();
	std::string meta = "M ";
	std::string details = "";

	meta.append(TextFormat("%s %d %d", mod.data(), Game::GetObjectCount(), p_count));
	for (int i = 0; i < p_count; i += 1) {
		details.append(TextFormat(" %d %d", Game::GetPlayerJointCount(i), Game::GetPlayerBodyCount(i)));
	}
	meta.append(TextFormat("%s", details.data()));

	std::ofstream tempframefile("replays/tempframefile.txt");
	tempframefile << meta << std::endl;
	tempframefile.close();

	std::ofstream tempreplayfile("replays/tempreplayfile.txt");
	tempreplayfile << meta << std::endl;
	tempreplayfile.close();

	Destroy();
}

void Replay::WriteFrameData(std::string data)
{
	std::ofstream tempframefile("replays/tempframefile.txt", std::ios::app);
	tempframefile << data << std::endl;
	tempframefile.close();
}

void Replay::WriteReplayData(std::string data)
{
	std::ofstream tempreplayfile("replays/tempreplayfile.txt", std::ios::app);
	tempreplayfile << data << std::endl;
	tempreplayfile.close();
}

void Replay::RecordFrame(int game_frame)
{
	std::string tempframe = "F ";
	tempframe.append(TextFormat("%d\n", game_frame));

	auto p_count = Game::GetPlayerCount();
	
	if (p_count < 1) return;

	auto& frame = *((FrameData*)frames + game_frame);

	frame.p_count = p_count;
	frame.players = storage->allocate(sizeof(FramePlayer)*p_count);

	auto& players = frame.players;

	for (auto& p : Game::GetPlayers()) {
		auto p_id = p.GetID();
		auto& player = *((FramePlayer*)players + p_id);
		
		player.j_count = Game::GetPlayerJointCount(p_id);
		player.b_count = Game::GetPlayerBodyCount(p_id);

		//player.J = storage->allocate(sizeof(uint8_t)*(player.j_count));
		player.Js = storage->allocate(sizeof(uint8_t)*(player.j_count));
		player.Jv = storage->allocate(sizeof(double)*(player.j_count*2));

		player.B = storage->allocate(sizeof(uint8_t)*(player.b_count));

		player.Q = storage->allocate(sizeof(double)*(player.b_count*4));
		player.P = storage->allocate(sizeof(double)*(player.b_count*3));
		player.L = storage->allocate(sizeof(double)*(player.b_count*3));
		player.A = storage->allocate(sizeof(double)*(player.b_count*3));

		std::string Js = "Js";
		std::string Jv = "Jv";

		std::string B = "B";
		std::string P = "P";
		std::string Q = "Q";
		std::string L = "L";
		std::string A = "A";

		for (auto& j : p.joint) {
			auto j_id = j.GetID();
			
			uint8_t state_byte = j.state + (j.state_alt << 2);
			Js.append(TextFormat(" %d", state_byte));

			*((uint8_t*)player.Js + j_id) = state_byte;

			Jv.append(TextFormat(" %f %f", j.frame_vel, j.frame_vel_alt));

			*((double*)player.Jv + (2*j_id + 0)) = j.frame_vel;
			*((double*)player.Jv + (2*j_id + 1)) = j.frame_vel_alt;
		}

		for (auto& b : p.body) {
			auto b_id = b.GetID();
			
			B.append(TextFormat(" %d", b.active));
			*((uint8_t*)player.B + b_id) = (uint8_t)b.active;

			Q.append(TextFormat(" %f %f %f %f",
				b.frame_orientation.w,
				b.frame_orientation.x,
				b.frame_orientation.y,
				b.frame_orientation.z
			));

			*((double*)player.Q + (4*b_id + 0)) = b.frame_orientation.w;
			*((double*)player.Q + (4*b_id + 1)) = b.frame_orientation.x;
			*((double*)player.Q + (4*b_id + 2)) = b.frame_orientation.y;
			*((double*)player.Q + (4*b_id + 3)) = b.frame_orientation.z;

			P.append(TextFormat(" %f %f %f",
				b.frame_position.x,
				b.frame_position.y,
				b.frame_position.z
			));

			*((double*)player.P + (3*b_id + 0)) = b.frame_position.x;
			*((double*)player.P + (3*b_id + 1)) = b.frame_position.y;
			*((double*)player.P + (3*b_id + 2)) = b.frame_position.z;

			L.append(TextFormat(" %f %f %f",
				b.frame_linear_vel.x,
				b.frame_linear_vel.y,
				b.frame_linear_vel.z
			));

			*((double*)player.L + (3*b_id + 0)) = b.frame_linear_vel.x;
			*((double*)player.L + (3*b_id + 1)) = b.frame_linear_vel.y;
			*((double*)player.L + (3*b_id + 2)) = b.frame_linear_vel.z;

			A.append(TextFormat(" %f %f %f",
				b.frame_angular_vel.x,
				b.frame_angular_vel.y,
				b.frame_angular_vel.z
			));

			*((double*)player.A + (3*b_id + 0)) = b.frame_angular_vel.x;
			*((double*)player.A + (3*b_id + 1)) = b.frame_angular_vel.y;
			*((double*)player.A + (3*b_id + 2)) = b.frame_angular_vel.z;
		}

		tempframe.append(TextFormat("%s\n", Js.c_str()));
		tempframe.append(TextFormat("%s\n", Jv.c_str()));

		tempframe.append(TextFormat("%s\n", B.c_str()));
		tempframe.append(TextFormat("%s\n", P.c_str()));
		tempframe.append(TextFormat("%s\n", Q.c_str()));
		tempframe.append(TextFormat("%s\n", L.c_str()));
		tempframe.append(TextFormat("%s\n", A.c_str()));
	}

	WriteFrameData(tempframe);
	WriteReplayData(tempframe);

	frame_count += 1;
}

void Replay::RecordFrame()
{
	RecordFrame(0);
}

void Replay::Begin()
{
	frame_count = 0;
}

void Replay::Play(int game_frame)
{
	auto& frame = *((FrameData*)frames + game_frame);

	if (game_frame > frame_count) return;

	for (int p_id = 0; p_id < frame.p_count; p_id += 1) {
		auto& p = *((FramePlayer*)frame.players + p_id);
	
		for (int j_id = 0; j_id < p.j_count; j_id += 1) {
			auto& Js = *((uint8_t*)p.Js + j_id);

			uint8_t state_alt = Js >> 2;
			uint8_t state = Js - (state_alt << 2);

			Game::TriggerPlayerJoint(p_id, j_id, (JointState)state, *((double*)p.Jv + j_id * 2 + 0));
			Game::TriggerPlayerJointAlt(p_id, j_id, (JointState)state_alt, *((double*)p.Jv + j_id * 2 + 1));
		}

		for (int b_id = 0; b_id < p.b_count; b_id += 1) {
			auto& B = *((uint8_t*)p.B + b_id);
			Game::SetBodyState(p_id, b_id, (bool)B);

			auto& Qw = *((double*)p.Q + b_id * 4 + 0);
			auto& Qx = *((double*)p.Q + b_id * 4 + 1);
			auto& Qy = *((double*)p.Q + b_id * 4 + 2);
			auto& Qz = *((double*)p.Q + b_id * 4 + 3);

			auto& Px = *((double*)p.P + b_id * 3 + 0);
			auto& Py = *((double*)p.P + b_id * 3 + 1);
			auto& Pz = *((double*)p.P + b_id * 3 + 2);

			auto& Lx = *((double*)p.L + b_id * 3 + 0);
			auto& Ly = *((double*)p.L + b_id * 3 + 1);
			auto& Lz = *((double*)p.L + b_id * 3 + 2);

			auto& Ax = *((double*)p.A + b_id * 3 + 0);
			auto& Ay = *((double*)p.A + b_id * 3 + 1);
			auto& Az = *((double*)p.A + b_id * 3 + 2);

			//Game::SetBodyLinearVel(p_id, b_id, p.L[p_id*b_id], p.L[p_id * b_id + 1], p.L[p_id * b_id + 2]);
			//Game::SetBodyAngularVel(p_id, b_id, p.A[p_id*b_id], p.A[p_id * (b_id + 1)], p.A[p_id * (b_id + 2)]);
			//dBodySetLinearVel(b.dBody, b.frame_linear_vel.x, b.frame_linear_vel.y, b.frame_linear_vel.z);
			//dBodySetAngularVel(b.dBody, b.frame_angular_vel.x, b.frame_angular_vel.y, b.frame_angular_vel.x);
		}
	}
}

void Replay::Load(std::string replay_name)
{
	std::string replay = "replays/";
	replay.append(replay_name);

	std::ifstream savedreplayfile(replay, std::ios::binary);

	char c;
	int i;

	for (i = 0; savedreplayfile.get(c); i += 1) {
		*((uint8_t*)storage + i) = (uint8_t)c;
	}

	savedreplayfile.close();
}

void Replay::Save(std::string replay_name)
{
	std::string replay = "replays/";
	replay.append(replay_name);

	std::ofstream savedreplayfile(replay.append(".rpl"), std::ios::binary);

	// FIXME: Don't Save Pointers Dumbass.

	for (int i = 0; i < storage->offset(); i += 1) {
		savedreplayfile << *((uint8_t*)storage + i);
	}

	savedreplayfile.close();
}

uint32_t Replay::GetFrameCount()
{
	return frame_count;
}
