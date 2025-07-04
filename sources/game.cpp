#include "game.h"
#include "replay.h"
#include "luau.h"
#include "api.h"

void Game::Init()
{
	dInitODE();
	s_step = 1.0E-2;
	s_world = dWorldCreate();

	API::Init();

	s_rules.max_contacts = 8;
	s_nearCallback = [](void* unsafe, dGeomID o1, dGeomID o2)
	{
		Game::NearCallback(o1, o2);
		API::NearCallback(s_collision);
	};
}

void Game::NewGame()
{
	API::NewGameCallback();
	dMass mass;

	s_rules = API::GetRules();
	s_objects = API::GetObjects();
	s_players = API::GetPlayers();

	s_state.game_frame = 0;

	s_state.freeze = true;
	s_state.freeze_time = GetTime();
	s_state.freeze_frames = 50;
	s_state.freeze_count = 0;

	s_state.step_frames = 0;
	s_state.step_count = 0;

	s_space = dHashSpaceCreate(0);
  	s_contactgroup = dJointGroupCreate(0);
	s_floor = dCreatePlane(s_space, 0, 0, 1, 0);

	dGeomSetCategoryBits(s_floor, 0b0001);
	dGeomSetCollideBits(s_floor, 0b0000);

  	dWorldSetGravity(s_world, s_rules.gravity.x, s_rules.gravity.y, s_rules.gravity.z);

	for (int i = 0; i < s_objects.length; i += 1) {
		auto&& o = s_objects[i];
		//o.create(s_world, s_space);
		//o.make_static(s_world);
		//o.set_category_bits();
		//o.set_collide_bits();
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

	for (int i = 0; i < s_players.length; i += 1) {
		auto&& p = s_players[i];
		p.joint_color = colors[i];

		//p.set_category_bits(0b0000, 0b0000);
		//p.set_collide_bits(0b0001, 0b0001);
		//p.set_offset();
		if (s_rules.engageheight) {
			///p.set_engageheight(s_rules.engageheight);
		}
		if (s_rules.engagedistance) {
			//p.set_engagedistance(s_rules.engagedistance,  i * (360/s_rules.numplayers));
		}

		//p.create(s_world, s_space);
		if (s_state.selected_player == "NONE") {
			s_state.selected_player = p.get_name();
		}
	}
	Replay::Record();
}

void Game::Quit()
{
	for (int i = 0; i < s_players.length; i += 1) {
		delete[] s_players[i].body.start;
		delete[] s_players[i].joint.start;
	}
	delete[] s_players.start;
	delete[] s_objects.start;
	API::Close();
	dJointGroupDestroy(s_contactgroup);
	dSpaceDestroy(s_space);
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

	dContact contact[s_rules.max_contacts];

	for (int i = 0; i < s_rules.max_contacts; i += 1) {
		contact[i].surface = (dSurfaceParameters) {
			.mode = dContactApprox1,
			.mu = s_rules.friction,
		};
	}

	if (int numc = dCollide(o1, o2, s_rules.max_contacts, &contact[0].geom, sizeof(dContact))) {
		for (int i = 0; i < numc; i += 1) {
			dJointID c = dJointCreateContact(s_world, s_contactgroup, contact + i);
			dJointAttach(c, b1, b2);
		}
	}
}

void Game::UpdateFreeze()
{
	API::FreezeCallback();

	s_state.freeze = true;
	s_state.freeze_time = GetTime();
	s_state.step_count = 0;

	for (int i = 0; i < s_objects.length; i += 1) {
		auto&& o = s_objects[i];
		o.update_freeze();
	}

	for (int i = 0; i < s_players.length; i += 1) {
		auto&& p = s_players[i];
		p.update_freeze();
	}
}

void Game::ReFreeze()
{
	s_state.freeze_count = 0;

	for (int i = 0; i < s_objects.length; i += 1) {
		auto&& o = s_objects[i];
		o.refreeze();
	}

	for (int i = 0; i < s_players.length; i += 1) {
		auto&& p = s_players[i];
		p.refreeze();
	}
}

void Game::Restart()
{
	for (int i = 0; i < s_objects.length; i += 1) {
		auto&& o = s_objects[i];
		o.reset();
	}
	
	for (int i = 0; i < s_players.length; i += 1) {
		auto&& p = s_players[i];
		p.reset();
	}
}

void Game::Reset()
{
	/*std::map<std::string, Body> new_object_map;
	objects = new_object_map;
	std::map<std::string, Player> new_player_map;
	players = new_player_map;*/

	dJointGroupDestroy(s_contactgroup);
	dSpaceDestroy(s_space);

	NewGame();
}


void Game::StepGame(int frame_count)
{
	API::StepCallback();
	s_state.freeze = false;
	s_state.step_frames = frame_count;
	ReFreeze();
}

void Game::Update(dReal dt)
{
	API::UpdateCallback(dt);
	return;
	if (!s_state.pause) {
		if (!s_state.freeze) {
			++s_state.game_frame;
			switch (s_state.gamemode) {
				case FREEPLAY: {
					if (++s_state.step_count >= s_state.step_frames) {
						UpdateFreeze();
					}

					Replay::Record(s_state.game_frame);
				} break;
				case REPLAY: {
					const auto& frames = Replay::Get();
					dReal size = frames.size();
					if (s_state.game_frame > size + 100) {
						//StartReplay();
					} else if (s_state.game_frame < size) {
						Replay::Play(s_state.game_frame);
					}
				} break;
			}
		} else {
			switch (s_state.gamemode) {
				case FREEPLAY: {
					if (++s_state.freeze_count >= s_state.freeze_frames) {
						ReFreeze();
					}
	
					if (s_rules.reaction_time != 0) {
						s_state.reaction_count = GetTime() - s_state.freeze_time;
						if (s_state.reaction_count >= s_rules.reaction_time) {
							StepGame(s_rules.turnframes);
						}
					}
				} break;
			}
		}

		dSpaceCollide(s_space, 0, s_nearCallback);
		dWorldStep(s_world, s_step);
		dJointGroupEmpty(s_contactgroup);
	}
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
	API::DrawCallback();
	DrawFloor();
	return;
	if (s_state.freeze) {
		for (int i = 0; i < s_objects.length; i += 1) {
			auto&& o = s_objects[i];
			o.draw_freeze();
			o.draw_ghost();
		}
				
		for (int i = 0; i < s_players.length; i += 1) {
			auto&& p = s_players[i];
			p.draw_freeze();
			p.draw_ghost();
		}
	} else {
		for (int i = 0; i < s_objects.length; i += 1) {
			auto&& o = s_objects[i];
			o.draw();
		}

		for (int i = 0; i < s_players.length; i += 1) {
			auto&& p = s_players[i];
			p.draw();
		}
	}
}

const char* Game::GetMod()
{
	return s_rules.mod.c_str();
}

int Game::GetGameFrame()
{
	return s_state.game_frame;
}

dReal Game::GetReactionTime()
{
	return s_rules.reaction_time;
}

dReal Game::GetReactionCount()
{
	return s_state.reaction_count;
}

bool Game::GetPause()
{
	return s_state.pause;
}

bool Game::GetFreeze()
{
	return s_state.freeze;
}

Gamerules Game::GetGamerules()
{
	return s_rules;
}

array<Body> Game::GetObjects()
{
	return s_objects;
}

array<Player> Game::GetPlayers()
{
	return s_players;
}

void Game::TogglePause()
{
	s_state.pause = s_state.pause == false;
}

/*
void Game::ToggleGhosts()
{
	for (auto& [player_name, p] : players) {
		if (player_name != state.selected_player) {
			p.toggle_ghost();
		}
	}
}

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

void Game::Start () {
	state.running = true;
};

bool Game::Running () {
	return state.running;
};

void Game::Loop () {
	int i = 1;
};
*/
