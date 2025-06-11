#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>
#include <cstring>
#include <ode/ode.h>
#include <game.hpp>
#include <body.hpp>
#include <joint.hpp>
#include <player.hpp>
#include <luau.hpp>
#include <api.hpp>
#include <api_raylib.hpp>
#include <api_raymath.hpp>
#include <api_gamerule.hpp>
#include <callbacks.hpp>

struct Console {
	char* messages[1024];
	char* last_message;
} console;

void ConsoleCallback(lua_State* L, const char* message)
{
	lua_getglobal(L, "Console");
	if (lua_istable(L, -1)) {
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (lua_isfunction(L, -1)) {
				lua_pushstring(L, message);
				if (lua_pcall(L, 1, 0, 0) == LUA_OK) {
				}
			}
		}
		lua_pop(L, 1);
	}
}

void ConsoleLog(lua_State* L, char* message)
{
	console.last_message = message;
	ConsoleCallback(L, console.last_message);
}

struct Window {
	int width;
	int height;
} window;

class FrameData : public FreezeData {
	public:
	std::map<std::string, Player> player;
};

std::map<int, FrameData> RecordedFrames;

static void nearCallback (void *, dGeomID o1, dGeomID o2)
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

	dContact contact[game.rules.max_contacts];

	int i;
	for (i = 0; i < game.rules.max_contacts; i++) {
		contact[i].surface.mode = dContactApprox1;
		contact[i].surface.mu = game.rules.friction;
	}

	if (int numc = dCollide(o1, o2, game.rules.max_contacts, &contact[0].geom, sizeof(dContact))) {
		for (i = 0; i < numc; i++) {
			dJointID c = dJointCreateContact(game.world, game.contactgroup, contact + i);
			dJointAttach(c, b1, b2);
		}
	}
}

int api_GetWindowSize(lua_State* L)
{
	int WindowSize[] = {window.width, window.height};
	lua_newtable(L);
	lua_pushnumber(L, WindowSize[0]);
	lua_setfield(L, -2, "width");
	lua_pushnumber(L, WindowSize[1]);
	lua_setfield(L, -2, "height");
	return 1;
}


void NewGame()
{
	dMass mass;

	game.state.game_frame = 0;

	game.state.freeze = true;
	game.state.freeze_time = GetTime();
	game.state.freeze_frames = 50;
	game.state.freeze_count = 0;

	game.state.step_frames = 0;
	game.state.step_count = 0;

	game.space = dHashSpaceCreate(0);
  	game.contactgroup = dJointGroupCreate(0);
	game.floor = dCreatePlane(game.space, 0, 0, 1, 0);

	dGeomSetCategoryBits(game.floor, 0b0001);
	dGeomSetCollideBits(game.floor, 0b0000);

  	dWorldSetGravity(
		game.world,
		game.rules.gravity[0],
		game.rules.gravity[1],
		game.rules.gravity[2]
	);

	for (auto& [object_name, o] : game.objects) {
		o.create();
		o.make_static();
	}

	for (auto& [player_name, p] : game.players) {
		p.create();
	}

	if (game.rules.engageheight) {
		for (auto& [player_name, p] : game.players) {
			for (auto& [body_name, b] : p.body) {
				b.position[2] = b.position[2] + game.rules.engageheight;
				b.freeze.position[2] = b.freeze.position[2] + game.rules.engageheight;
			}
		
			for (auto& [joint_name, j] : p.joint) {
				j.position[2] = j.position[2] + game.rules.engageheight;
				j.freeze.position[2] = j.freeze.position[2] + game.rules.engageheight;
			}
		}
	}

	if (game.rules.engagedistance) {
		int i = 0;
		for (auto& [player_name, p] : game.players) {
			Matrix m = MatrixRotateZ(DEG2RAD * (360/game.rules.numplayers) * i);
			Quaternion q = QuaternionFromMatrix(m);
			Quaternion iq = QuaternionInvert(q);
			for (auto& [body_name, b] : p.body) {
				Quaternion p = QuaternionMultiply(
						iq,
						(Quaternion){
						b.position[0],
						b.position[1] + game.rules.engagedistance,
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
							j.position[1] + game.rules.engagedistance,
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
	}

}

void ResetGame()
{
	std::map<std::string, Body> new_object_map;
	game.objects = new_object_map;
	std::map<std::string, Player> new_player_map;
	game.players = new_player_map;

	dJointGroupDestroy(game.contactgroup);
	dSpaceDestroy(game.space);

	NewGame();
}

void UpdateFreeze()
{
	game.state.freeze = true;
	game.state.freeze_time = GetTime();
	game.state.step_count = 0;

	for (auto& [object_name, o] : game.objects) {
		o.update_freeze();
	}

	for (auto& [player_name, p] : game.players) {
		p.update_freeze();
	}
}

void ReFreeze()
{
	game.state.freeze_count = 0;

	for (auto& [object_name, o] : game.objects) {
		o.refreeze();
	}

	for (auto& [player_name, p] : game.players) {
		p.ReFreeze();
	}
}

void DrawFloor()
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

void ToggleGhosts()
{
	for (auto& [player_name, p] : game.players) {
		if (player_name != game.state.selected_player) {
			p.toggle_ghost();
		}
	}
}

void DrawFrame()
{
	DrawFloor();
	if (game.state.freeze) {
		for (auto& [object_name, o] : game.objects) {
			o.draw_freeze();
			o.draw_ghost();
		}
				
		for (auto& [player_name, p] : game.players) {
			p.draw_freeze();
			p.draw_ghost();
		}
	} else {
		for (auto& [object_name, o] : game.objects) {
			o.draw();
		}
		for (auto& [player_name, p] : game.players) {
			p.draw();
		}
	}
}

void EndGame()
{
	dJointGroupDestroy(game.contactgroup);
	dSpaceDestroy(game.space);
	dCloseODE();
}

void StepGame(int frame_count)
{
	game.state.freeze = false;
	game.state.step_frames = frame_count;
	ReFreeze();
}

void Restart()
{
	for (auto& [object_name, o] : game.objects) {
		dBodySetPosition(
			o.dBody,
			o.position[0],
			o.position[1],
			o.position[2]);
		dBodySetQuaternion(o.dBody, o.orientation);
		dBodySetLinearVel(o.dBody, 0.00, 0.00, 0.00);
		dBodySetAngularVel(o.dBody, 0.00, 0.00, 0.00);

		o.freeze.position[0] = o.position[0];
		o.freeze.position[1] = o.position[1];
		o.freeze.position[2] = o.position[2];

		o.freeze.orientation[0] = o.orientation[0];
		o.freeze.orientation[1] = o.orientation[1];
		o.freeze.orientation[2] = o.orientation[2];
		o.freeze.orientation[3] = o.orientation[3];

		o.freeze.linear_vel[0] = 0.00;
		o.freeze.linear_vel[1] = 0.00;
		o.freeze.linear_vel[2] = 0.00;

		o.freeze.angular_vel[0] = 0.00;
		o.freeze.angular_vel[1] = 0.00;
		o.freeze.angular_vel[2] = 0.00;
	}
	
	for (auto& [player_name, p] : game.players) {
		for (auto& [body_name, b] : p.body) {
			dBodySetPosition(
				b.dBody,
				b.position[0],
				b.position[1],
				b.position[2]);
			dBodySetQuaternion(b.dBody, b.orientation);
			dBodySetLinearVel(b.dBody, 0.00, 0.00, 0.00);
			dBodySetAngularVel(b.dBody, 0.00, 0.00, 0.00);
	
			b.freeze.position[0] = b.position[0];
			b.freeze.position[1] = b.position[1];
			b.freeze.position[2] = b.position[2];
	
			b.freeze.orientation[0] = b.orientation[0];
			b.freeze.orientation[1] = b.orientation[1];
			b.freeze.orientation[2] = b.orientation[2];
			b.freeze.orientation[3] = b.orientation[3];
	
			b.freeze.linear_vel[0] = 0.00;
			b.freeze.linear_vel[1] = 0.00;
			b.freeze.linear_vel[2] = 0.00;
	
			b.freeze.angular_vel[0] = 0.00;
			b.freeze.angular_vel[1] = 0.00;
			b.freeze.angular_vel[2] = 0.00;
		}
		
		for (auto& [joint_name, j] : p.joint) {
			dGeomSetPosition(
				j.dGeom,
				j.position[0],
				j.position[1],
				j.position[2]);
			dGeomSetQuaternion(j.dGeom, j.orientation);
			dBodySetLinearVel(j.dBody, 0.00, 0.00, 0.00);
			dBodySetAngularVel(j.dBody, 0.00, 0.00, 0.00);
	
			j.freeze.position[0] = j.position[0];
			j.freeze.position[1] = j.position[1];
			j.freeze.position[2] = j.position[2];
	
			j.freeze.orientation[0] = j.orientation[0];
			j.freeze.orientation[1] = j.orientation[1];
			j.freeze.orientation[2] = j.orientation[2];
			j.freeze.orientation[3] = j.orientation[3];
	
			j.freeze.linear_vel[0] = 0.00;
			j.freeze.linear_vel[1] = 0.00;
			j.freeze.linear_vel[2] = 0.00;
	
			j.freeze.angular_vel[0] = 0.00;
			j.freeze.angular_vel[1] = 0.00;
			j.freeze.angular_vel[2] = 0.00;
		}
	}
}

void CameraRotateZClockwise(Camera3D *camera, Vector3 *CameraZoom, Vector3 *CameraOffset)
{
	Matrix m = MatrixRotateZ(DEG2RAD*10);
	Quaternion q = QuaternionFromMatrix(m);
	Quaternion iq = QuaternionInvert(q);
	Quaternion p = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraOffset->x,
				CameraOffset->y,
				CameraOffset->z,
				0.0f
	});

	p = QuaternionMultiply(p, q);

	CameraOffset->x = p.x;
	CameraOffset->y = p.y;
	CameraOffset->z = p.z;

	Quaternion p2 = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraZoom->x,
				CameraZoom->y,
				CameraZoom->z,
				0.0f
	});

	p2 = QuaternionMultiply(p2, q);

	CameraZoom->x = p2.x;
	CameraZoom->y = p2.y;
	CameraZoom->y = p2.z;
}

void CameraRotateZCClockwise(Camera3D *camera, Vector3 *CameraZoom, Vector3 *CameraOffset)
{
	Matrix m = MatrixRotateZ(DEG2RAD*-10);
	Quaternion q = QuaternionFromMatrix(m);
	Quaternion iq = QuaternionInvert(q);
	Quaternion p = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraOffset->x,
				CameraOffset->y,
				CameraOffset->z,
				0.0f
	});

	p = QuaternionMultiply(p, q);

	CameraOffset->x = p.x;
	CameraOffset->y = p.y;
	CameraOffset->z = p.z;

	Quaternion p2 = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraZoom->x,
				CameraZoom->y,
				CameraZoom->z,
				0.0f
	});

	p2 = QuaternionMultiply(p2, q);

	CameraZoom->x = p2.x;
	CameraZoom->y = p2.y;
	CameraZoom->z = p2.z;
}

// TODO: FIX
void CameraRotateXClockwise(Camera3D *camera, Vector3 *CameraZoom, Vector3 *CameraOffset)
{
	Matrix m = MatrixRotateX(DEG2RAD*10);
	Quaternion q = QuaternionFromMatrix(m);
	Quaternion iq = QuaternionInvert(q);
	Quaternion p = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraOffset->x,
				CameraOffset->y,
				CameraOffset->z,
				0.0f
	});

	p = QuaternionMultiply(p, q);

	CameraOffset->x = p.x;
	CameraOffset->y = p.y;
	CameraOffset->z = p.z;

	Quaternion p2 = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraZoom->x,
				CameraZoom->y,
				CameraZoom->z,
				0.0f
	});

	p2 = QuaternionMultiply(p2, q);

	CameraZoom->x = p2.x;
	CameraZoom->y = p2.y;
	CameraZoom->y = p2.z;
}

// TODO: FIX
void CameraRotateXCClockwise(Camera3D *camera, Vector3 *CameraZoom, Vector3 *CameraOffset)
{
	Matrix m = MatrixRotateX(DEG2RAD*-10);
	Quaternion q = QuaternionFromMatrix(m);
	Quaternion iq = QuaternionInvert(q);
	Quaternion p = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraOffset->x,
				CameraOffset->y,
				CameraOffset->z,
				0.0f
	});

	p = QuaternionMultiply(p, q);

	CameraOffset->x = p.x;
	CameraOffset->y = p.y;
	CameraOffset->z = p.z;

	Quaternion p2 = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraZoom->x,
				CameraZoom->y,
				CameraZoom->z,
				0.0f
	});

	p2 = QuaternionMultiply(p2, q);

	CameraZoom->x = p2.x;
	CameraZoom->y = p2.y;
	CameraZoom->z = p2.z;
}

void CameraZoomIn(Camera3D *camera, Vector3 *CameraZoom, Vector3 *CameraOffset)
{
	CameraOffset->x = 0.9f * CameraOffset->x;
	CameraOffset->y = 0.9f * CameraOffset->y;
	CameraOffset->z = 0.9f * CameraOffset->z; 
}

void CameraZoomOut(Camera3D *camera, Vector3 *CameraZoom, Vector3 *CameraOffset)
{
	CameraOffset->x = 1.1f * CameraOffset->x;
	CameraOffset->y = 1.1f * CameraOffset->y;
	CameraOffset->z = 1.1f * CameraOffset->z;
}

void UpdatePlaycam(Camera3D *camera, Vector3 *CameraOffset)
{
	int size = game.players[game.state.selected_player].body.size(); 
	if (size > 0) {
		float x = 0.00;
		float y = 0.00;
		float z = 0.00;
	
		for (auto const [body_name, b] : game.players[game.state.selected_player].body) {
			if (game.state.freeze) {
				x = x + b.freeze.position[0];
				y = y + b.freeze.position[1];
				z = z + b.freeze.position[2];
			} else {
				const dReal *position = dBodyGetPosition(b.dBody);
				x = x + position[0];
				y = y + position[1];
				z = z + position[2];
			}
		}

		camera->target = (Vector3){ x/size, y/size, z/size };
		camera->position = (Vector3){
			CameraOffset->x + x/size,
			CameraOffset->y + y/size,
			CameraOffset->z + z/size,
		};
	} else {
		camera->target = (Vector3){ 0.00, 0.00, 0.00 };
		camera->position = (Vector3){
			CameraOffset->x,
			CameraOffset->y,
			CameraOffset->z
		};
	}
}

void SelectPlayer(Camera3D Camera, Ray MouseRay, RayCollision MouseCollision)
{
	RayCollision collision = { 0 };
	MouseRay = GetMouseRay(GetMousePosition(), Camera);
	for (auto& [player_name, p] : game.players) {
		for (auto& [body_name, b] : p.body) {
			MouseCollision = b.collide_mouse_ray(MouseRay, MouseCollision);
			if (MouseCollision.hit) {
				collision = MouseCollision;
				game.state.selected_player = player_name;
				break;
			}
		}
		for (auto& [joint_name, j] : p.joint) {
			MouseCollision = j.collide_mouse_ray(MouseRay, MouseCollision);
			if (MouseCollision.hit) {
				collision = MouseCollision;
				game.state.selected_player = player_name;
				break;
			}
		}
	}
}

void SelectJoint(Camera3D Camera, Ray MouseRay, RayCollision MouseCollision)
{
	RayCollision collision = { 0 };
	MouseRay = GetMouseRay(GetMousePosition(), Camera);
	for (auto& [joint_name, j] : game.players[game.state.selected_player].joint) {
		MouseCollision = j.collide_mouse_ray(MouseRay, MouseCollision);
		if (MouseCollision.hit) {
			collision = MouseCollision;
			game.state.selected_joint = j.name;
			j.select = true;
			break;
		} else {
			game.state.selected_joint = "NONE";
			j.select = false;
		}
	}
}

void RecordFrame(int game_frame)
{
	std::ofstream tempframefile("tempframefile.txt");
	tempframefile << "FRAME " << game_frame << "\n";
	for (auto const& [player_name, p] : game.players) {
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
				b.freeze.position[0] << " " <<
				b.freeze.position[1] << " " <<
				b.freeze.position[2] << std::endl;
			tempframefile << "QAT " <<
				player_name << " " <<
				body_name << " " <<
				b.freeze.orientation[0] << " " <<
				b.freeze.orientation[1] << " " <<
				b.freeze.orientation[2] << " " <<
				b.freeze.orientation[3] << std::endl;
			tempframefile << "LINVEL " <<
				player_name << " " <<
				body_name << " " <<
				b.freeze.linear_vel[0] << " " <<
				b.freeze.linear_vel[1] << " " <<
				b.freeze.linear_vel[2] << std::endl;
			tempframefile << "ANGVEL " <<
				player_name << " " <<
				body_name << " " <<
				b.freeze.angular_vel[0] << " " <<
				b.freeze.angular_vel[1] << " " <<
				b.freeze.angular_vel[2] << std::endl;
		}
	}
	tempframefile.close();

	std::ofstream tempreplayfile("tempreplayfile.txt", std::ios::app);
	tempreplayfile << "FRAME " << game_frame << "\n";
	for (auto const& [player_name, p] : game.players) {
		tempreplayfile << "PLAYER " << player_name << "\n";
		RecordedFrames[game_frame].player[player_name] = p;
		for (auto const& [joint_name, j] : p.joint) {
			tempreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
		}
	}
	tempreplayfile.close();
}

void PlayFrame(int game_frame)
{
	FrameData frame = RecordedFrames[game_frame];
	for (auto& [player_name, p] : frame.player) {
		for (auto& [joint_name, j] : p.joint) {
			switch(j.state) {
				case RELAX: {
					j.state = RELAX;
					game.players[p.name].joint[j.name]
					.TriggerPassiveState(0.00);
				} break;
				case HOLD: {
					j.state = HOLD;
					game.players[p.name].joint[j.name]
					.TriggerPassiveState(j.strength);
				} break;
				case FORWARD: {
					j.state = FORWARD;
					game.players[p.name].joint[j.name]
					.TriggerActiveState(1.00);
				} break; 
				case BACKWARD: {
					j.state = BACKWARD;
					game.players[p.name].joint[j.name]
					.TriggerActiveState(-1.00);
				} break;
			}
		
			switch(j.state_alt) {
				case RELAX: {
					j.state_alt = RELAX;
					game.players[p.name].joint[j.name]
					.TriggerPassiveStateAlt(0.00);
				} break;
				case HOLD: {
					j.state_alt = HOLD;
					game.players[p.name].joint[j.name]
					.TriggerPassiveStateAlt(j.strength_alt);
				} break;
				case FORWARD: {
					j.state_alt = FORWARD;
					game.players[p.name].joint[j.name]
					.TriggerActiveStateAlt(1.00);
				} break;
				case BACKWARD: {
					j.state_alt = BACKWARD;
					game.players[p.name].joint[j.name]
					.TriggerActiveStateAlt(-1.00);
				} break;
			}
		}
	}
}

void EditReplay()
{
	game.state.gamemode = FREEPLAY;

	UpdateFreeze();

	std::ofstream tempframefile("tempframefile.txt");
	tempframefile << "FRAME " << game.state.game_frame << "\n";
	for (auto const& [player_name, p] : game.GetPlayers()) {
		tempframefile << "PLAYER " << player_name << "\n";
		for (auto const& [joint_name, j] : p.joint) {
			tempframefile << j.name << " " << j.state << " " << j.state_alt << "\n";
		}
	}
	tempframefile.close();

	std::ofstream tempreplayfile("tempreplayfile.txt");
	for (auto const& [game_frame, frame] : RecordedFrames) {
		if (game_frame > game.state.game_frame) {
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

void SaveReplay()
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

void StartFreeplay()
{
	game.state.gamemode = FREEPLAY;

	for (auto& [player_name, p] : game.GetPlayers()) {
		p.RelaxAll();
		p.RelaxAllAlt();
	}

	game.state.game_frame = 0;
	game.state.reaction_count = 0;
	game.state.freeze_count = 0;
	game.state.step_count = 0;
	
	Restart();

	UpdateFreeze();

	std::ofstream tempreplayfile("tempreplayfile.txt");
	tempreplayfile << "FRAME 0\n";
	for (auto const& [player_name, p] : game.GetPlayers()) {
		tempreplayfile << "PLAYER " << player_name << "\n";
		for (auto const& [joint_name, j] : p.joint) {
			RecordedFrames[0].player[player_name].joint[joint_name] = j;
			tempreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
		}
	}
	tempreplayfile.close();
}

void StartReplay()
{
	game.state.gamemode = REPLAY;

	for (auto& [player_name, p] : game.GetPlayers()) {
		p.RelaxAll();
		p.RelaxAllAlt();
	}

	game.state.freeze = false;
	game.state.game_frame = 0;
	game.state.reaction_count = 0;
	game.state.freeze_count = 0;
	game.state.step_count = 0;
	
	Restart();
}


void UpdateFrame()
{
	if (!game.state.pause) {
		if (!game.state.freeze) {
			++game.state.game_frame;
			switch (game.state.gamemode) {
				case FREEPLAY: {
					if (++game.state.step_count >= game.state.step_frames) {
						UpdateFreeze();
					}

					RecordFrame(game.state.game_frame);
				} break;
				case REPLAY: {
					dReal size = RecordedFrames.size();
					if (game.state.game_frame > size + 100) {
						StartReplay();
					} else if (game.state.game_frame < size) {
						PlayFrame(game.state.game_frame);
					}
				} break;
			}
		} else {
			switch (game.state.gamemode) {
				case FREEPLAY: {
					if (++game.state.freeze_count >= game.state.freeze_frames) {
						ReFreeze();
					}
	
					if (game.rules.reaction_time != 0) {
						game.state.reaction_count = GetTime() - game.state.freeze_time;
						if (game.state.reaction_count >= game.rules.reaction_time) {
							StepGame(game.rules.turnframes);
						}
					}
				} break;
			}
		}

		dSpaceCollide(game.space, 0, &nearCallback);
		dWorldStep(game.world, game.step);
		dJointGroupEmpty(game.contactgroup);
	}
}

int main()
{
	window.width = 1600;
	window.height = 900;
	SetTraceLogLevel(LOG_ERROR);
	InitWindow(window.width, window.height, "TOBAS");
	SetTargetFPS(60);
	SetExitKey(0);

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaopen_api_main(L);
	luaopen_api_raylib(L);
	luaopen_api_raymath(L);
	luaopen_api_gamerule(L);

	SetupCallbacks(L);

	api_set(L, api_dofile, "dofile");
	api_set(L, api_GetWindowSize, "GetWindowSize");
	
	#define MAX_FILEPATH_RECORDED   4096
	#define MAX_FILEPATH_SIZE       2048

	int filePathCounter = 0;
    	char *filePaths[MAX_FILEPATH_RECORDED] = { 0 };
	for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
		filePaths[i] = (char *)RL_CALLOC(MAX_FILEPATH_SIZE, 1);
	}

	if (luau_dofile(L, "scripts/init.luau", "=dofile:init.luau") != LUA_OK) {
		luaL_error(L, "Error: %s\n", lua_tostring(L, -1));
	}

	luaL_sandbox(L);
	luaL_sandboxthread(L);

	Camera3D camera = { 0 };
	camera.up = (Vector3){ 0.00, 0.00, 1.00 };
	camera.fovy = 45.00;
	camera.projection = CAMERA_PERSPECTIVE;

	Vector3 CameraOffset = (Vector3){ 0.00, -5.00, 0.00 };
	Vector3 CameraZoom = (Vector3){ 0.00, 0.00, 0.00 };

	Ray MouseRay = { 0 };
	RayCollision MouseCollision = { 0 };

	dInitODE2(0);
	game.world = dWorldCreate();

	NewGame();

	while (!WindowShouldClose()) {
		SetWindowTitle(TextFormat("TOBAS %dFPS", GetFPS()));

		UpdatePlaycam(&camera, &CameraOffset);

		SelectJoint(camera, MouseRay, MouseCollision);
		
		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			if (IsKeyDown(KEY_W))
				CameraRotateXClockwise(&camera, &CameraZoom, &CameraOffset);

			if (IsKeyDown(KEY_A))
				CameraRotateZClockwise(&camera, &CameraZoom, &CameraOffset);

			if (IsKeyDown(KEY_S))
				CameraRotateXCClockwise(&camera, &CameraZoom, &CameraOffset);

			if (IsKeyDown(KEY_D))
				CameraRotateZCClockwise(&camera, &CameraZoom, &CameraOffset);
		} else {
			if (IsKeyDown(KEY_W))
				CameraZoomIn(&camera, &CameraZoom, &CameraOffset);

			if (IsKeyDown(KEY_A))
				CameraRotateZClockwise(&camera, &CameraZoom, &CameraOffset);

			if (IsKeyDown(KEY_S))
				CameraZoomOut(&camera, &CameraZoom, &CameraOffset);

			if (IsKeyDown(KEY_D))
				CameraRotateZCClockwise(&camera, &CameraZoom, &CameraOffset);
		}

		if (IsKeyPressed(KEY_Z)) {
			if (IsKeyDown(KEY_LEFT_CONTROL)) {
				if (game.state.freeze && game.state.gamemode == FREEPLAY) {
					PlayFrame(game.state.game_frame - 1);
				}
			} else {
				if (game.state.selected_joint != "NONE") {
					ReFreeze();
					if (IsKeyDown(KEY_LEFT_SHIFT)) {
						game.players[game.state.selected_player]
						.joint[game.state.selected_joint]
						.ToggleActiveStateAlt();
					} else {
						game.players[game.state.selected_player]
						.joint[game.state.selected_joint]
						.ToggleActiveState();
					}
				}
			}
		}

		if (IsKeyPressed(KEY_X)) {
			if (game.state.selected_joint != "NONE") {
				ReFreeze();
				if (IsKeyDown(KEY_LEFT_SHIFT)) {
					game.players[game.state.selected_player]
					.joint[game.state.selected_joint]
					.TogglePassiveStateAlt();
				} else {
					game.players[game.state.selected_player]
					.joint[game.state.selected_joint]
					.TogglePassiveState();
				}
			}
		}

		if (IsKeyPressed(KEY_C)) {
			game.players[game.state.selected_player].TogglePlayerPassiveStatesAlt();
			game.players[game.state.selected_player].TogglePlayerPassiveStates();
		}

		if (IsMouseButtonPressed(0)) {
			SelectPlayer(camera, MouseRay, MouseCollision);
			if (game.state.selected_joint != "NONE") {
				ReFreeze();
				if (IsKeyDown(KEY_LEFT_SHIFT)) {
					game.players[game.state.selected_player]
					.joint[game.state.selected_joint]
					.CycleStateAlt();
				} else {
					game.players[game.state.selected_player]
					.joint[game.state.selected_joint]
					.CycleState();
				}
			}
		}

		if (IsKeyPressed(KEY_SPACE)) {
			if (game.state.freeze && game.state.gamemode == FREEPLAY) {
				if (IsKeyDown(KEY_LEFT_SHIFT)) {
					StepGame(1);
				} else {
					StepGame(game.rules.turnframes);
				}
			} else if (!game.state.freeze && game.state.gamemode == REPLAY) {
				StartFreeplay();
			}
		}

		if (IsKeyPressed(KEY_E)) {
			if (!game.state.freeze && game.state.gamemode == REPLAY) {
				EditReplay();
			}
		}

		if (IsKeyPressed(KEY_F)) {
			SaveReplay();
		}

		if (IsKeyPressed(KEY_R)) {
			StartReplay();
		}

		if (IsKeyPressed(KEY_P)) {
			game.TogglePause();
		}

		if (IsKeyPressed(KEY_G)) {
			ToggleGhosts();
		}

		if (IsKeyPressed(KEY_ESCAPE)) {
			ResetGame();
		}

		if (IsKeyPressed(KEY_ESCAPE)) {
			//GetKeyPressed();
			//KeyPressedCallback(L);
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			MouseButtonPressedCallback(L);
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			MouseButtonDownCallback(L);
		}

		if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
			MouseButtonUpCallback(L);
		}

		if (IsFileDropped()) {
			FilePathList dropped_files = LoadDroppedFiles();
			for (int i = 0, offset = filePathCounter; i < (int)dropped_files.count; i++) {
				if (filePathCounter < (MAX_FILEPATH_RECORDED - 1)) {
					TextCopy(filePaths[offset + i], dropped_files.paths[i]);
					filePathCounter++;
				}
			}
			UnloadDroppedFiles(dropped_files);
			FileDroppedCallback(L, filePaths[0]);
        	}

		UpdateFrame();
	
		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
				DrawFrame();
			EndMode3D();
			Draw2DCallback(L);
		EndDrawing();
	}

	for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        	RL_FREE(filePaths[i]);
    	}

	EndGame();
	CloseWindow();
	lua_close(L);

	return 0;
}
