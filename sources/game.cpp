#include "game.h"
#include "replay.h"
#include "luau.h"
#include "api.h"

static void Start () {
	Game::state.running = true;
};

static bool Running () {
	return Game::state.running;
};

static void Loop () {
	int i = 1;
};

void Game::Init()
{
	dInitODE();
	world = dWorldCreate();
	step = 1.0E-2;

	API::Init();

	nearCallback = [](void* unsafe, dGeomID o1, dGeomID o2) {
		Game::NearCallback(o1, o2);
		//API::NearCallback(collision);
	};
}

void Game::NewGame()
{
	dMass mass;

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

	for (int i = 0; i < objects.size(); i += 1) {
		auto&& o = objects[i];
		o.create(world, space);
		o.make_static(world);
		o.set_category_bits();
		o.set_collide_bits();
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

	for (int i = 0; i < players.size(); i += 1) {
		auto&& p = players[i];
		p.joint_color = colors[i];

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
	}

	Replay::RecordFrame();

	API::NewGameCallback();
};

void Game::Quit()
{
	for (int i = 0; i < players.size(); i += 1) {
		delete[] players[i].body.data();
		delete[] players[i].joint.data();
	}

	delete[] players.data();
	delete[] objects.data();

	API::Close();
	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(space);
	dCloseODE();

}

void Game::NearCallback(dGeomID o1, dGeomID o2)
{
	unsigned long cat1 = dGeomGetCategoryBits(o1);
	unsigned long col1 = dGeomGetCollideBits(o1);

	unsigned long cat2 = dGeomGetCategoryBits(o2);
	unsigned long col2 = dGeomGetCollideBits(o2);

 	if (!((cat1 & col2) || (cat2 & col1))) return;

	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	dContact contact[rules.max_contacts];

	for (int i = 0; i < rules.max_contacts; i += 1) {
		contact[i].surface = (dSurfaceParameters) {
			.mode = dContactApprox1,
			.mu = rules.friction,
		};
	}

	if (int numc = dCollide(o1, o2, rules.max_contacts, &contact[0].geom, sizeof(dContact))) {
		for (int i = 0; i < numc; i += 1) {
			dJointID c = dJointCreateContact(world, contactgroup, contact + i);
			dJointAttach(c, b1, b2);
		}
	}
}

void Game::UpdateFreeze()
{
	state.freeze = true;
	state.freeze_time = GetTime();
	state.step_count = 0;

	for (int i = 0; i < objects.size(); i += 1) {
		auto&& o = objects[i];
		o.update_freeze();
	}

	for (int i = 0; i < players.size(); i += 1) {
		auto&& p = players[i];
		p.update_freeze();
	}

	API::FreezeCallback();
}

void Game::ReFreeze()
{
	state.freeze_count = 0;

	for (int i = 0; i < objects.size(); i += 1) {
		auto&& o = objects[i];
		o.refreeze();
	}

	for (int i = 0; i < players.size(); i += 1) {
		auto&& p = players[i];
		p.refreeze();
	}
}

void Game::Restart()
{
	for (int i = 0; i < objects.size(); i += 1) {
		auto&& o = objects[i];
		o.reset();
	}
	
	for (int i = 0; i < players.size(); i += 1) {
		auto&& p = players[i];
		p.reset();
	}
}

void Game::Reset()
{
	/*std::map<std::string, Body> new_object_map;
	objects = new_object_map;
	std::map<std::string, Player> new_player_map;
	players = new_player_map;*/

	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(space);

	NewGame();
}


void Game::StepGame(int frame_count)
{
	state.freeze = false;
	state.step_frames = frame_count;

	ReFreeze();

	API::StepCallback();
}

void Game::Update(dReal dt)
{
	if (!state.pause) {
		if (!state.freeze) {
			++state.game_frame;
			switch (state.gamemode) {
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
			switch (state.gamemode) {
				case FREEPLAY: {
					if (++state.freeze_count >= state.freeze_frames) {
		LOG(state.freeze_count)
						ReFreeze();
					}
	
					if (rules.reaction_time != 0) {
						state.reaction_count = GetTime() - state.freeze_time;
						if (state.reaction_count >= rules.reaction_time) {
							StepGame(rules.turnframes);
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
}

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
}

void Game::Draw()
{
	DrawFloor();
	if (state.freeze) {
		for (int i = 0; i < objects.size(); i += 1) {
			auto&& o = objects[i];
			o.draw_freeze();
			o.draw_ghost();
		}
				
		for (int i = 0; i < players.size(); i += 1) {
			auto&& p = players[i];
			p.draw_freeze();
			p.draw_ghost();
		}
	} else {
		for (int i = 0; i < objects.size(); i += 1) {
			auto&& o = objects[i];
			o.draw();
		}

		for (int i = 0; i < players.size(); i += 1) {
			auto&& p = players[i];
			p.draw();
		}
	}

	API::DrawCallback();
}


bool Game::GetPause()
{
	return state.pause;
}

bool Game::GetFreeze()
{
	return state.freeze;
}

Gamerules Game::GetGamerules()
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

array<Body> Game::GetObjects()
{
	return objects;
}

Player Game::GetPlayer(PlayerID player_id)
{
	return players[player_id];
}

Player Game::GetSelectedPlayer()
{
	return players[state.selected_player];
}

PlayerID Game::GetSelectedPlayerID()
{
	return state.selected_player;
}

array<Player> Game::GetPlayers()
{
	return players;
}

void Game::TogglePause()
{
	state.pause = state.pause == false;
}


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
/*
void Game::StartFreeplay()
{
	state.gamemode = FREEPLAY;

	for (auto& [player_name, p] : GetPlayers()) {
		p.RelaxAll();
		p.RelaxAllAlt();
	}

	state.game_frame = 0;
	state.reaction_count = 0;
	state.freeze_count = 0;
	state.step_count = 0;
	
	Restart();

	UpdateFreeze();

	Replay::Record();
}
*/
/*
void Game::StartReplay()
{
	state.gamemode = REPLAY;

	for (auto& [player_name, p] : GetPlayers()) {
		p.RelaxAll();
		p.RelaxAllAlt();
	}

	state.freeze = false;
	state.game_frame = 0;
	state.reaction_count = 0;
	state.freeze_count = 0;
	state.step_count = 0;
	
	Restart();
}
*/
