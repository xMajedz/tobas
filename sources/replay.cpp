#include "replay.h"

#include <fstream>

using namespace raylib;

void Replay::Init()
{
	storage = new Arena(6*1024*1024);

	uintptr_t uptr = (uintptr_t)new int;

	//*(int*)uptr = 9;

	delete (int*)uptr;
}

void Replay::Close()
{
	delete storage;
}

void Replay::Destroy()
{
	for (auto& [frame, data] : frames) {
		for (int i = 0; i < data.p_count; i += 1) {
			auto& player = data.players[i];
			player.~FramePlayer();
		}
	}

	storage->clear();
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

	std::ofstream tempframefile("tempframefile.txt");
	tempframefile << meta << std::endl;
	tempframefile.close();

	std::ofstream tempreplayfile("tempreplayfile.txt");
	tempreplayfile << meta << std::endl;
	tempreplayfile.close();

	Destroy();
}

void Replay::WriteFrameData(std::string data)
{
	std::ofstream tempframefile("tempframefile.txt", std::ios::app);
	tempframefile << data << std::endl;
	tempframefile.close();
}

void Replay::WriteReplayData(std::string data)
{
	std::ofstream tempreplayfile("tempreplayfile.txt", std::ios::app);
	tempreplayfile << data << std::endl;
	tempreplayfile.close();
}

void Replay::RecordFrame(int game_frame)
{
	std::string tempframe = "F ";
	tempframe.append(TextFormat("%d\n", game_frame));

	frames[game_frame].p_count = Game::GetPlayerCount();
	
	if (frames[game_frame].p_count > 0) {
		frames[game_frame].players = (FramePlayer*)storage->allocate(sizeof(FramePlayer)*frames[game_frame].p_count);
	} else {
		return ;
	}

	auto& players = frames[game_frame].players;

	for (auto& p : Game::GetPlayers()) {
		auto p_id = p.GetID();
	
		players->j_count = Game::GetPlayerJointCount(p_id);
		players->b_count = Game::GetPlayerBodyCount(p_id);

		players[p_id].J = (uint8_t*)storage->allocate(sizeof(uint8_t)*players->j_count);
		players[p_id].B = (uint8_t*)storage->allocate(sizeof(uint8_t)*players->b_count);

		players[p_id].Q = (double*)storage->allocate(sizeof(double)*4*players->b_count);
		players[p_id].P = (double*)storage->allocate(sizeof(double)*3*players->b_count);
		players[p_id].L = (double*)storage->allocate(sizeof(double)*3*players->b_count);
		players[p_id].A = (double*)storage->allocate(sizeof(double)*3*players->b_count);

		std::string J = "J";
		std::string B = "B";
		std::string P = "P";
		std::string Q = "Q";
		std::string L = "L";
		std::string A = "A";

		for (auto& j : p.joint) {
			auto j_id = j.GetID();
			uint8_t state_byte = j.state + (j.state_alt << 2);
			J.append(TextFormat(" %d", state_byte));
			players[p_id].J[j_id] = state_byte;
		}

		for (auto& b : p.body) {
			auto b_id = b.GetID();
			players[p_id].B[b_id] = (uint8_t)b.active;
			B.append(TextFormat(" %d", b.active));
			
			players[p_id].Q[4 * b_id ] = b.frame_orientation.w;
			players[p_id].Q[4 * b_id + 1] = b.frame_orientation.x;
			players[p_id].Q[4 * b_id + 2] = b.frame_orientation.y;
			players[p_id].Q[4 * b_id + 3] = b.frame_orientation.z;
			Q.append(TextFormat(" %f %f %f %f",
				b.frame_orientation.w,
				b.frame_orientation.x,
				b.frame_orientation.y,
				b.frame_orientation.z
			));

			players[p_id].P[3 * b_id] = b.frame_position.x;
			players[p_id].P[3 * b_id + 1] = b.frame_position.y;
			players[p_id].P[3 * b_id + 2] = b.frame_position.z;
			P.append(TextFormat(" %f %f %f",
				b.frame_position.x,
				b.frame_position.y,
				b.frame_position.z
			));

			players[p_id].L[3 * b_id] = b.frame_linear_vel.x;
			players[p_id].L[3 * b_id + 1] = b.frame_linear_vel.y;
			players[p_id].L[3 * b_id + 2] = b.frame_linear_vel.z;
			L.append(TextFormat(" %f %f %f",
				b.frame_linear_vel.x,
				b.frame_linear_vel.y,
				b.frame_linear_vel.z
			));

			players[p_id].A[3 * b_id] = b.frame_angular_vel.x;
			players[p_id].A[3 * b_id + 1] = b.frame_angular_vel.y;
			players[p_id].A[3 * b_id + 2] = b.frame_angular_vel.z;
			A.append(TextFormat(" %f %f %f",
				b.frame_angular_vel.x,
				b.frame_angular_vel.y,
				b.frame_angular_vel.z
			));
		}

		tempframe.append(TextFormat("%s\n", J.c_str()));
		tempframe.append(TextFormat("%s\n", B.c_str()));
		tempframe.append(TextFormat("%s\n", P.c_str()));
		tempframe.append(TextFormat("%s\n", Q.c_str()));
		tempframe.append(TextFormat("%s\n", L.c_str()));
		tempframe.append(TextFormat("%s\n", A.c_str()));
	}

	WriteFrameData(tempframe);
	WriteReplayData(tempframe);

	players->~FramePlayer();
}

void Replay::RecordFrame()
{
	RecordFrame(0);
}

void Replay::Play(int game_frame)
{
	auto& frame = frames[game_frame];
	for (int p_id = 0; p_id < frame.p_count; p_id += 1) {
		auto& p = frame.players[p_id];
		for (int j_id = 0; j_id < p.j_count; j_id += 1) {
			auto& J = p.J[j_id];
			uint8_t state_alt = J >> 2;
			uint8_t state = J - (state_alt << 2);

			Game::TriggerPlayerJointState(p_id, j_id, (JointState)state);
			Game::TriggerPlayerJointStateAlt(p_id, j_id, (JointState)state_alt);
		}

		for (int b_id = 0; b_id < p.b_count; b_id += 1) {
			//Game::SetBodyState(p_id, b_id, p.B[b_id]);
			//Game::SetBodyLinearVel(p_id, b_id, p.L[p_id*b_id], p.L[p_id * b_id + 1], p.L[p_id * b_id + 2]);
			//Game::SetBodyAngularVel(p_id, b_id, p.A[p_id*b_id], p.A[p_id * (b_id + 1)], p.A[p_id * (b_id + 2)]);
			//dBodySetLinearVel(b.dBody, b.frame_linear_vel.x, b.frame_linear_vel.y, b.frame_linear_vel.z);
			//dBodySetAngularVel(b.dBody, b.frame_angular_vel.x, b.frame_angular_vel.y, b.frame_angular_vel.x);
		}
	}
}

void Replay::Save(std::string replay_name)
{
	std::ofstream savedreplayfile(replay_name.append(".rpl2"));
	/*for (auto const& [game_frame, frame] : RecordedFrames) {
		savedreplayfile << "FRAME " << game_frame << "\n";
		for (auto const& [player_name, p] : frame.player) {
			savedreplayfile << "PLAYER " << player_name << "\n";
			for (auto const& [joint_name, j] : p.joint) {
				savedreplayfile << j.name << " " << j.state << " " << j.state_alt << "\n";
			}
		}
	}*/
	savedreplayfile.close();
}

void Replay::Save()
{
	Save("savedreplayname");
}

FrameData Replay::Get(int frame)
{
	return frames[frame];
}

std::map<int, FrameData>& Replay::Get()
{
	return frames;
}
