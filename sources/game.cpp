#include <luau.h>
#include <raylib.h>
#include <api_callbacks.h>
#include <game.h>
#include <fstream>
#include <iostream>

#define PRINT(X) std::cout << "-- TEST " << X << " --" << std::endl;
#define PRINT_A PRINT("A")
#define PRINT_B PRINT("B")
#define PRINT_C PRINT("C")

void Game::NearCallback(dGeomID o1, dGeomID o2)
{
	unsigned long cat1 = dGeomGetCategoryBits(o1);
	unsigned long col1 = dGeomGetCollideBits(o1);

	unsigned long cat2 = dGeomGetCategoryBits(o2);
	unsigned long col2 = dGeomGetCollideBits(o2);

 	if (!((cat1 & col2) || (cat2 & col1))) {
		return;
	}

	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	dContact contact[rules.max_contacts];
	collision.contacts = contact[0];

	for (int i = 0; i < rules.max_contacts; i++) {
		contact[i].surface.mode = dContactApprox1;
		contact[i].surface.mu = rules.friction;
	}

	if (int numc = dCollide(o1, o2, rules.max_contacts, &contact[0].geom, sizeof(dContact))) {
		for (int i = 0; i < numc; i++) {
			dJointID c = dJointCreateContact(world, contactgroup, contact + i);
			dJointAttach(c, b1, b2);
		}
	}
}

Gamerules Game::GetGamerules()
{
	return rules;
}

std::map<std::string, Player> Game::GetPlayers()
{
	return players;
}

std::map<std::string, Body> Game::GetObjects()
{
	return objects;
}

void Game::TogglePause()
{
	state.pause = state.pause == false;
}

bool Game::GetPause()
{
	return state.pause;
}

bool Game::GetFreeze()
{
	return state.freeze;
}

void Game::NewGame()
{
	dMass mass;

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

  	dWorldSetGravity(
		world,
		rules.gravity[0],
		rules.gravity[1],
		rules.gravity[2]
	);

	for (auto& [object_name, o] : objects) {
		o.create(world, space);
		o.make_static(world);
	}

	for (auto& [player_name, p] : players) {
		p.create(world, space);
	}

	/*if (rules.engageheight) {
		for (auto& [player_name, p] : players) {
			for (auto& [body_name, b] : p.body) {
				b.position[2] = b.position[2] + rules.engageheight;
				b.freeze.position[2] = b.freeze.position[2] + rules.engageheight;
			}
		
			for (auto& [joint_name, j] : p.joint) {
				j.position[2] = j.position[2] + rules.engageheight;
				j.freeze.position[2] = j.freeze.position[2] + rules.engageheight;
			}
		}
	}

	if (rules.engagedistance) {
		int i = 0;
		for (auto& [player_name, p] : players) {
			Matrix m = MatrixRotateZ(DEG2RAD * (360/rules.numplayers) * i);
			Quaternion q = QuaternionFromMatrix(m);
			Quaternion iq = QuaternionInvert(q);
			for (auto& [body_name, b] : p.body) {
				Quaternion p = QuaternionMultiply(
						iq,
						(Quaternion){
						b.position[0],
						b.position[1] + rules.engagedistance,
						b.position[2],
						0.00
				});
				p = QuaternionMultiply(p, q);
				
				Quaternion b_q = QuaternionMultiply(
						q,
						(Quaternion){
						b.orientation[1],
						b.orientation[2],
						b.orientation[3],
						b.orientation[0],
				});

				b.orientation[0] = b_q.w;
				b.freeze.orientation[0] = b.orientation[0];
				b.orientation[1] = b_q.x;
				b.freeze.orientation[1] = b.orientation[1];
				b.orientation[2] = b_q.y;
				b.freeze.orientation[2] = b.orientation[2];
				b.orientation[3] = b_q.z;
				b.freeze.orientation[3] = b.orientation[3];

				b.position[0] = p.x;
				b.freeze.position[0] = b.position[0];
				b.position[1] = p.y;
				b.freeze.position[1] = b.position[1];
			}

			for (auto& [joint_name, j] : p.joint) {
				Quaternion p = QuaternionMultiply(
						iq,
						(Quaternion){
							j.position[0],
							j.position[1] + rules.engagedistance,
							j.position[2],
							0.00
				});
				p = QuaternionMultiply(p, q);

				Quaternion j_q = QuaternionMultiply(
						q,
						(Quaternion){
						j.orientation[1],
						j.orientation[2],
						j.orientation[3],
						j.orientation[0],
				});

				j.orientation[0] = j_q.w;
				j.freeze.orientation[0] = j.orientation[0];
				j.orientation[1] = j_q.x;
				j.freeze.orientation[1] = j.orientation[1];
				j.orientation[2] = j_q.y;
				j.freeze.orientation[2] = j.orientation[2];
				j.orientation[3] = j_q.z;
				j.freeze.orientation[3] = j.orientation[3];

				j.position[0] = p.x;
				j.freeze.position[0] = j.position[0];
				j.position[1] = p.y;
				j.freeze.position[1] = j.position[1];
			}
		i++;
		}
	}*/

}

void Game::ResetGame()
{
	std::map<std::string, Body> new_object_map;
	objects = new_object_map;
	std::map<std::string, Player> new_player_map;
	players = new_player_map;

	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(space);

	NewGame();
}

void Game::UpdateFreeze()
{
	state.freeze = true;
	state.freeze_time = GetTime();
	state.step_count = 0;

	for (auto& [object_name, o] : objects) {
		o.update_freeze();
	}

	for (auto& [player_name, p] : players) {
		p.update_freeze();
	}
}

void Game::ReFreeze()
{
	state.freeze_count = 0;

	for (auto& [object_name, o] : objects) {
		o.ReFreeze();
	}

	for (auto& [player_name, p] : players) {
		p.ReFreeze();
	}
}

void Game::DrawFloor()
{
	rlPushMatrix();
	float angle;
	Vector3 axis;
	Matrix m = MatrixRotateX(DEG2RAD*90);
	Quaternion q = QuaternionFromMatrix(m);
	QuaternionToAxisAngle(q, &axis, &angle);
	rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);
	DrawGrid(20, 1.0f);
	rlPopMatrix();
}

void Game::ToggleGhosts()
{
	for (auto& [player_name, p] : players) {
		if (player_name != state.selected_player) {
			p.toggle_ghost();
		}
	}
}

void Game::DrawFrame()
{
	DrawFloor();
	if (state.freeze) {
		for (auto& [object_name, o] : objects) {
			o.draw_freeze();
			o.draw_ghost();
		}
				
		for (auto& [player_name, p] : players) {
			p.draw_freeze();
			p.draw_ghost();
		}
	} else {
		for (auto& [object_name, o] : objects) {
			o.draw();
		}

		for (auto& [player_name, p] : players) {
			p.draw();
		}
	}
}

void Game::EndGame()
{
	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(space);
	dCloseODE();
}

void Game::StepGame(int frame_count)
{
	state.freeze = false;
	state.step_frames = frame_count;
	ReFreeze();
}

void Game::Restart()
{
	for (auto& [object_name, o] : objects) {
		dBodySetPosition(
			o.dBody,
			o.position.x,
			o.position.y,
			o.position.z
		);

		dQuaternion q = {
			o.orientation.w,
			o.orientation.x,
			o.orientation.y,
			o.orientation.z,
		};

		dBodySetQuaternion(o.dBody, q);

		dBodySetLinearVel(o.dBody, 0.00, 0.00, 0.00);
		dBodySetAngularVel(o.dBody, 0.00, 0.00, 0.00);

		o.freeze.position.x = o.position.x;
		o.freeze.position.y = o.position.y;
		o.freeze.position.z = o.position.z;

		o.freeze.orientation.w = o.orientation.w;
		o.freeze.orientation.x = o.orientation.x;
		o.freeze.orientation.y = o.orientation.y;
		o.freeze.orientation.z = o.orientation.z;

		o.freeze.linear_vel.x = 0.00;
		o.freeze.linear_vel.y = 0.00;
		o.freeze.linear_vel.z = 0.00;

		o.freeze.angular_vel.x = 0.00;
		o.freeze.angular_vel.y = 0.00;
		o.freeze.angular_vel.z = 0.00;
	}
	
	for (auto& [player_name, p] : players) {
		for (auto& [body_name, b] : p.body) {
			dBodySetPosition(
				b.dBody,
				b.position.x,
				b.position.y,
				b.position.z);

			dQuaternion q = {
				b.orientation.w,
				b.orientation.x,
				b.orientation.y,
				b.orientation.z,
			};

			dBodySetQuaternion(b.dBody, q);

			dBodySetLinearVel(b.dBody, 0.00, 0.00, 0.00);
			dBodySetAngularVel(b.dBody, 0.00, 0.00, 0.00);
	
			b.freeze.position.x = b.position.x;
			b.freeze.position.y = b.position.y;
			b.freeze.position.z = b.position.z;
	
			b.freeze.orientation.w = b.orientation.w;
			b.freeze.orientation.x = b.orientation.x;
			b.freeze.orientation.y = b.orientation.y;
			b.freeze.orientation.z = b.orientation.z;
	
			b.freeze.linear_vel.x = 0.00;
			b.freeze.linear_vel.y = 0.00;
			b.freeze.linear_vel.z = 0.00;
	
			b.freeze.angular_vel.x = 0.00;
			b.freeze.angular_vel.y = 0.00;
			b.freeze.angular_vel.z = 0.00;
		}
		
		for (auto& [joint_name, j] : p.joint) {
			dGeomSetPosition(
				j.dGeom,
				j.position.x,
				j.position.y,
				j.position.z);

			dQuaternion q = {
				j.orientation.w,
				j.orientation.x,
				j.orientation.y,
				j.orientation.z,
			};

			dGeomSetQuaternion(j.dGeom, q);

			dBodySetLinearVel(j.dBody, 0.00, 0.00, 0.00);
			dBodySetAngularVel(j.dBody, 0.00, 0.00, 0.00);
	
			j.freeze.position.x = j.position.x;
			j.freeze.position.y = j.position.y;
			j.freeze.position.z = j.position.z;
	
			j.freeze.orientation.w = j.orientation.w;
			j.freeze.orientation.x = j.orientation.x;
			j.freeze.orientation.y = j.orientation.y;
			j.freeze.orientation.z = j.orientation.z;
	
			j.freeze.linear_vel.x = 0.00;
			j.freeze.linear_vel.y = 0.00;
			j.freeze.linear_vel.z = 0.00;
	
			j.freeze.angular_vel.x = 0.00;
			j.freeze.angular_vel.y = 0.00;
			j.freeze.angular_vel.z = 0.00;
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

void Game::RecordFrame(int game_frame)
{
	std::ofstream tempframefile("tempframefile.txt");
	tempframefile << "FRAME " << game_frame << "\n";
	for (auto const& [player_name, p] : players) {
		RecordedFrames[game_frame].player[player_name] = p;
		for (auto const& [joint_name, j] : p.joint) {
			tempframefile << "JOINT " <<
				player_name << " " <<
				joint_name << " " <<
				j.state << " " <<
				j.state_alt << std::endl;
		}
		for (auto const& [body_name, b] : p.body) {
			tempframefile << "POS " <<
				player_name << " " <<
				body_name << " " <<
				b.freeze.position.x << " " <<
				b.freeze.position.y << " " <<
				b.freeze.position.z << std::endl;
			tempframefile << "QAT " <<
				player_name << " " <<
				body_name << " " <<
				b.freeze.orientation.w << " " <<
				b.freeze.orientation.x << " " <<
				b.freeze.orientation.y << " " <<
				b.freeze.orientation.z << std::endl;
			tempframefile << "LINVEL " <<
				player_name << " " <<
				body_name << " " <<
				b.freeze.linear_vel.x << " " <<
				b.freeze.linear_vel.y << " " <<
				b.freeze.linear_vel.z << std::endl;
			tempframefile << "ANGVEL " <<
				player_name << " " <<
				body_name << " " <<
				b.freeze.angular_vel.x << " " <<
				b.freeze.angular_vel.y << " " <<
				b.freeze.angular_vel.z << std::endl;
		}
	}
	tempframefile.close();

	std::ofstream tempreplayfile("tempreplayfile.txt", std::ios::app);
	tempreplayfile << "FRAME " << game_frame << "\n";
	for (auto const& [player_name, p] : players) {
		tempreplayfile << "PLAYER " << player_name << "\n";
		RecordedFrames[game_frame].player[player_name] = p;
		for (auto const& [joint_name, j] : p.joint) {
			tempreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
		}
	}
	tempreplayfile.close();
}

void Game::PlayFrame(int game_frame)
{
	FrameData frame = RecordedFrames[game_frame];
	for (auto& [player_name, p] : frame.player) {
		for (auto& [joint_name, j] : p.joint) {
			switch(j.state) {
				case RELAX: {
					j.state = RELAX;
					players[p.name].joint[j.name]
					.TriggerPassiveState(0.00);
				} break;
				case HOLD: {
					j.state = HOLD;
					players[p.name].joint[j.name]
					.TriggerPassiveState(j.strength);
				} break;
				case FORWARD: {
					j.state = FORWARD;
					players[p.name].joint[j.name]
					.TriggerActiveState(1.00);
				} break; 
				case BACKWARD: {
					j.state = BACKWARD;
					players[p.name].joint[j.name]
					.TriggerActiveState(-1.00);
				} break;
			}
		
			switch(j.state_alt) {
				case RELAX: {
					j.state_alt = RELAX;
					players[p.name].joint[j.name]
					.TriggerPassiveStateAlt(0.00);
				} break;
				case HOLD: {
					j.state_alt = HOLD;
					players[p.name].joint[j.name]
					.TriggerPassiveStateAlt(j.strength_alt);
				} break;
				case FORWARD: {
					j.state_alt = FORWARD;
					players[p.name].joint[j.name]
					.TriggerActiveStateAlt(1.00);
				} break;
				case BACKWARD: {
					j.state_alt = BACKWARD;
					players[p.name].joint[j.name]
					.TriggerActiveStateAlt(-1.00);
				} break;
			}
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

void Game::SaveReplay()
{
	std::ofstream savedreplayfile("savedreplayfile.txt");
	for (auto const& [game_frame, frame] : RecordedFrames) {
		savedreplayfile << "FRAME " << game_frame << "\n";
		for (auto const& [player_name, p] : frame.player) {
			savedreplayfile << "PLAYER " << player_name << "\n";
			for (auto const& [joint_name, j] : p.joint) {
				savedreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
			}
		}
	}
	savedreplayfile.close();
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

	std::ofstream tempreplayfile("tempreplayfile.txt");
	tempreplayfile << "FRAME 0\n";
	for (auto const& [player_name, p] : GetPlayers()) {
		tempreplayfile << "PLAYER " << player_name << "\n";
		for (auto const& [joint_name, j] : p.joint) {
			RecordedFrames[0].player[player_name].joint[joint_name] = j;
			tempreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
		}
	}
	tempreplayfile.close();
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

void Game::UpdateFrame()
{
	if (!state.pause) {
		if (!state.freeze) {
			++state.game_frame;
			switch (state.gamemode) {
				case FREEPLAY: {
					if (++state.step_count >= state.step_frames) {
						UpdateFreeze();
					}

					RecordFrame(state.game_frame);
				} break;
				case REPLAY: {
					dReal size = RecordedFrames.size();
					if (state.game_frame > size + 100) {
						StartReplay();
					} else if (state.game_frame < size) {
						PlayFrame(state.game_frame);
					}
				} break;
			}
		} else {
			switch (state.gamemode) {
				case FREEPLAY: {
					if (++state.freeze_count >= state.freeze_frames) {
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
}

void Game::UpdatePlaycam(Gamecam* gamecam)
{
	Player* selected_player;
	if (state.selected_player != "NONE") {
		selected_player = &(players[state.selected_player]);
	}
	int size = selected_player->body.size(); 
	if (size > 0) {
		float x = 0.00;
		float y = 0.00;
		float z = 0.00;
	
		for (auto const [body_name, b] : selected_player->body) {
			if (state.freeze) {
				x = x + b.freeze.position.x;
				y = y + b.freeze.position.y;
				z = z + b.freeze.position.z;
			} else {
				const dReal *position = dBodyGetPosition(b.dBody);
				x = x + position[0];
				y = y + position[1];
				z = z + position[2];
			}
		}

		gamecam->camera.target = (Vector3){ x/size, y/size, z/size };
		gamecam->camera.position = (Vector3){
			gamecam->CameraOffset.x + x/size,
			gamecam->CameraOffset.y + y/size,
			gamecam->CameraOffset.z + z/size,
		};
	} else {
		gamecam->camera.target = (Vector3){ 0.00, 0.00, 0.00 };
		gamecam->camera.position = (Vector3){
			gamecam->CameraOffset.x,
			gamecam->CameraOffset.y,
			gamecam->CameraOffset.z
		};
	}

	if (IsKeyDown(KEY_LEFT_SHIFT)) {
		if (IsKeyDown(KEY_W))
			gamecam->CameraRotateXClockwise();

		if (IsKeyDown(KEY_A))
			gamecam->CameraRotateZClockwise();

		if (IsKeyDown(KEY_S))
			gamecam->CameraRotateXCClockwise();

		if (IsKeyDown(KEY_D))
			gamecam->CameraRotateZCClockwise();
	} else {
		if (IsKeyDown(KEY_W))
			gamecam->CameraZoomIn();

		if (IsKeyDown(KEY_A))
			gamecam->CameraRotateZClockwise();

		if (IsKeyDown(KEY_S))
			gamecam->CameraZoomOut();

		if (IsKeyDown(KEY_D))
			gamecam->CameraRotateZCClockwise();
	}
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

void Console::log(lua_State* L, char* message)
{
	last_message = message;
	//ConsoleCallback(L, last_message);
};
