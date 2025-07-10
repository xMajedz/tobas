#include "game.h"
#include "replay.h"
#include "luau.h"
#include "api.h"

bool Game::Running ()
{
	return state.running;
};

static void Start ()
{
	Game::state.running = true;
};

static void Loop ()
{
	int i = 1;
};

void Game::Init()
{
	dInitODE();
	world = dWorldCreate();

	step = 1.0E-2;

	API::Init();
	
	API::SetCallback("NewGame", "init", [](lua_State* L) {
		Replay::RecordFrame();
		return 1;
	});

	API::SetCallback("Console", "init", [](lua_State* L) {
		//const char* message = lua_tostring(L, -1);
		return 1;
	});

	nearCallback = [](void* unsafe, dGeomID o1, dGeomID o2) {
		Game::NearCallback(o1, o2);
		//API::NearCallback(collision);
	};

	state.running = false;
};

void Game::NewGame()
{
	rules = API::GetRules();
	rules.max_contacts = 8;

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
		o.create(world, space);
		o.set_category_bits();
		o.set_collide_bits();
		o.make_static(world);
	}

	Color colors[] = {
		MAROON,
		DARKBLUE,
		DARKGREEN,
		DARKPURPLE,
	};

	unsigned long cat_bits[][2] = {
		{0b000000000010, 0b000000000100},
		{0b000000001000, 0b000000010000},
		{0b000000100000, 0b000001000000},
		{0b000010000000, 0b000100000000},
	};

	unsigned long col_bits[][2] = {
		{
			cat_bits[1][0]|cat_bits[2][0]|cat_bits[3][0],
			cat_bits[1][1]|cat_bits[2][1]|cat_bits[3][1],
		},
		{
			cat_bits[0][0]|cat_bits[2][0]|cat_bits[3][0],
			cat_bits[0][1]|cat_bits[2][1]|cat_bits[3][1],
		},
		{
			cat_bits[0][0]|cat_bits[1][0]|cat_bits[3][0],
			cat_bits[0][1]|cat_bits[1][1]|cat_bits[3][1],
		},
		{
			cat_bits[0][0]|cat_bits[1][0]|cat_bits[2][0],
			cat_bits[0][1]|cat_bits[1][1]|cat_bits[2][1],
		},
	};

	int i = 0;
	for (auto& p : players) {
		p.joint_color = colors[i];
		p.ghost_color = colors[i];
		p.ghost_color.a = 55;

		p.set_category_bits(0b0000, 0b0000);
		p.set_collide_bits(0b0001, 0b0001);
		p.set_offset();

		if (rules.engageheight) {
			p.set_engageheight(rules.engageheight);
		}

		if (rules.engagedistance) {
			p.set_engagedistance(rules.engagedistance,  i * (360/rules.numplayers));
		}

		p.create(world, space);

		if (0 > state.selected_player) {
			state.selected_player = 0;
		}
		i += 1;
	}


	state.running = true;

	API::NewGameCallback();
};

void Game::Quit()
{
	API::Close();
	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(space);
	dCloseODE();
};

void Game::NearCallback(dGeomID o1, dGeomID o2)
{
	uint32_t cat1 = dGeomGetCategoryBits(o1);
	uint32_t col1 = dGeomGetCollideBits(o1);

	uint32_t cat2 = dGeomGetCategoryBits(o2);
	uint32_t col2 = dGeomGetCollideBits(o2);

 	if (!((cat1 & col2) || (cat2 & col1))) {
		return;
	}

	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	dContact contacts[rules.max_contacts];

	for (auto& contact : contacts) {
		contact.surface = (dSurfaceParameters) {
			//.mode = dContactApprox1|dContactSoftERP|dContactSoftCFM,
			//.soft_erp = 0.420,
			//.soft_cfm = 0.420,
			.mode = dContactApprox1,
			.mu = rules.friction,
		};
	}

	if (int numc = dCollide(o1, o2, rules.max_contacts, &contacts->geom, sizeof(dContact))) {
		for (int i = 0; i < numc; i += 1) {
			dJointID c = dJointCreateContact(world, contactgroup, contacts + i);
			dJointAttach(c, b1, b2);
		}
	}
};

void Game::UpdateFreeze()
{
	state.freeze = true;
	state.freeze_time = GetTime();
	state.step_count = 0;
	
	for (auto& o : objects) {
		o.update_freeze();
	}

	for (auto& p : players) {
		p.update_freeze();
	}

	API::FreezeCallback();
};

void Game::Refreeze()
{
	state.freeze_count = 0;

	for (auto& o : objects) {
		o.refreeze();
	}

	for (auto& p : players) {
		p.refreeze();
	}
};

void Game::Restart()
{
	for (auto& o : objects) {
		o.reset();
	}
	
	for (auto& p : players) {
		p.reset();
	}
};

void Game::Reset()
{
	objects.clear();
	players.clear();

	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(space);

	NewGame();
};

void Game::Step(int frame_count)
{
	state.freeze = false;
	state.step_frames = frame_count;

	Refreeze();

	API::StepCallback();
};

void Game::Step()
{
	Game::Step(1);
};

void Game::Update(dReal dt)
{
	if (!state.pause) {
		if (!state.freeze) {
			++state.game_frame;
			switch (state.mode) {
				case FREEPLAY: {
					if (++state.step_count >= state.step_frames) {
						UpdateFreeze();
					}

					Replay::RecordFrame(state.game_frame);
				} break;
				case REPLAY: {
					const auto& frames = Replay::Get();
					dReal size = frames.size();
					if (state.game_frame > size + 100) {
						//StartReplay();
					} else if (state.game_frame < size) {
						Replay::Play(state.game_frame);
					}
				} break;
			}
		} else {
			switch (state.mode) {
				case FREEPLAY: {
					if (++state.freeze_count >= state.freeze_frames) {
						Refreeze();
					}
	
					if (rules.reaction_time != 0) {
						state.reaction_count = GetTime() - state.freeze_time;
						if (state.reaction_count >= rules.reaction_time) {
							Step(rules.turnframes);
						}
					}
				} break;
			}
		}


		dSpaceCollide(space, 0, nearCallback);
		dWorldStep(world, step);
		dJointGroupEmpty(contactgroup);
	}

	API::UpdateCallback(dt);
};

void Game::DrawFloor()
{
	rlPushMatrix();
	Vector3 axis;
	float angle;
	Quaternion q = QuaternionFromMatrix(MatrixRotateX(DEG2RAD*90));
	QuaternionToAxisAngle(q, &axis, &angle);
	rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);
	DrawGrid(20, 1.0f);
	rlPopMatrix();
};

void Game::Draw()
{
	DrawFloor();

	for (auto& o : objects) {
		o.draw(state.freeze);
	}

	for (auto& p : players) {
		p.draw(state.freeze);
	}

	API::DrawCallback();
};

bool Game::GetPause()
{
	return state.pause;
};

bool Game::GetFreeze()
{
	return state.freeze;
};

Gamemode Game::GetGamemode()
{
	return state.mode;
};

Gamerules Game::GetGamerules()
{
	return rules;
};

std::string_view Game::GetMod()
{
	return rules.mod;
};

int Game::GetGameFrame()
{
	return state.game_frame;
};

dReal Game::GetReactionTime()
{
	return rules.reaction_time;
};

dReal Game::GetReactionCount()
{
	return state.reaction_count;
};

size_t Game::GetObjectCount()
{
	return objects.size();
};

size_t Game::GetPlayerCount()
{
	return players.size();
};

size_t Game::GetPlayerBodyCount(PlayerID player_id)
{
	return players[player_id].body.size();
};

size_t Game::GetPlayerJointCount(PlayerID player_id)
{
	return players[player_id].joint.size();
};

std::vector<Body> Game::GetObjects()
{
	return objects;
};

const Player& Game::GetPlayer(PlayerID player_id)
{
	return players[player_id];
};

Player Game::GetSelectedPlayer()
{
	return players[state.selected_player];
};

PlayerID Game::GetSelectedPlayerID()
{
	return state.selected_player;
};

Joint Game::GetSelectedJoint()
{
	return players[state.selected_player].joint[state.selected_joint];
};

JointID Game::GetSelectedJointID()
{
	return state.selected_joint;
};

std::vector<Player> Game::GetPlayers()
{
	return players;
};

void Game::SetSelectedPlayer(PlayerID player_id)
{
	state.selected_player = player_id;
};

void Game::TogglePause()
{
	state.pause = state.pause == false;
};

void Game::ToggleGhosts()
{
	for (int i = 0; i < players.size(); i += 1) {
		auto&& p = players[i];
		if (0 > state.selected_player) {
			p.toggle_ghost();
		} else if (i != state.selected_player) {
			p.toggle_ghost();
		}
	}
}

float Window::GetWidth()
{
	return width;
};

float Window::GetHeight()
{
	return height;
};
/*
void Game::SelectPlayer(Camera3D Camera, Ray MouseRay, RayCollision MouseCollision)
{
	RayCollision collision = { 0 };
	MouseRay = GetMouseRay(GetMousePosition(), Camera);
	for (auto& [player_name, p] : players) {
		for (auto& [body_name, b] : p.body) {
			MouseCollision = b.collide_mouse_ray(MouseRay, MouseCollision);
			if (MouseCollision.hit) {
				collision = MouseCollision;
				state.selected_player = player_name;
				break;
			}
		}
		for (auto& [joint_name, j] : p.joint) {
			MouseCollision = j.collide_mouse_ray(MouseRay, MouseCollision);
			if (MouseCollision.hit) {
				collision = MouseCollision;
				state.selected_player = player_name;
				break;
			}
		}
	}
}
*/
/*
void Game::SelectJoint(Camera3D Camera, Ray MouseRay, RayCollision MouseCollision)
{
	RayCollision collision = { 0 };
	MouseRay = GetMouseRay(GetMousePosition(), Camera);
	for (auto& [joint_name, j] : players[state.selected_player].joint) {
		MouseCollision = j.collide_mouse_ray(MouseRay, MouseCollision);
		if (MouseCollision.hit) {
			collision = MouseCollision;
			state.selected_joint = j.name;
			j.select = true;
			break;
		} else {
			state.selected_joint = "NONE";
			j.select = false;
		}
	}
}
*/
/*
void Game::EditReplay()
{
	state.gamemode = FREEPLAY;

	UpdateFreeze();

	std::ofstream tempframefile("tempframefile.txt");
	tempframefile << "FRAME " << state.game_frame << "\n";
	for (auto const& [player_name, p] : GetPlayers()) {
		tempframefile << "PLAYER " << player_name << "\n";
		for (auto const& [joint_name, j] : p.joint) {
			tempframefile << j.name << " " << j.state << " " << j.state_alt << "\n";
		}
	}
	tempframefile.close();

	std::ofstream tempreplayfile("tempreplayfile.txt");
	for (auto const& [game_frame, frame] : RecordedFrames) {
		if (game_frame > state.game_frame) {
			break;
		} else {
			tempreplayfile << "FRAME " << game_frame << "\n";
			for (auto const& [player_name, p] : frame.player) {
				tempreplayfile << "FRAME " << player_name << "\n";
				for (auto const& [joint_name, j] : p.joint) {
					tempreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
				}
			}
		}
	}
	tempreplayfile.close();
}
*/

void Game::ModeFreeplay()
{
	state.mode = FREEPLAY;

	for (auto& p : players) {
		p.RelaxAll();
		p.RelaxAllAlt();
	}

	state.game_frame = 0;
	state.reaction_count = 0;
	state.freeze_count = 0;
	state.step_count = 0;
	
	Restart();

	UpdateFreeze();

	Replay::RecordFrame();
};

void Game::ModeReplay()
{
	state.mode = REPLAY;

	for (auto& p : players) {
		p.RelaxAll();
		p.RelaxAllAlt();
	}

	state.freeze = false;
	state.game_frame = 0;
	state.reaction_count = 0;
	state.freeze_count = 0;
	state.step_count = 0;
	
	Restart();
};

