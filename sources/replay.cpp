#include "replay.h"

#include <fstream>

using namespace raylib;

void Replay::RecordFrame(int game_frame)
{
	std::string tempframe = "F";
	tempframe.append(TextFormat(" %d\n",game_frame));
	for (auto&  p : Game::GetPlayers()) {
		std::string J = "J";
		std::string P = "P";
		std::string Q = "Q";
		std::string L = "L";
		std::string A = "A";

		frames[game_frame].players.push_back(p);

		for (auto& j : p.joint) {
			J.append(TextFormat(" %d %d", j.state, j.state_alt));
		}
		tempframe.append(TextFormat("%s\n", J.c_str()));

		for (auto& b : p.body) {
			P.append(TextFormat(" %f %f %f",
				b.freeze_position.x,
				b.freeze_position.y,
				b.freeze_position.z
			));

			Q.append(TextFormat(" %f %f %f %f",
				b.freeze_orientation.w,
				b.freeze_orientation.x,
				b.freeze_orientation.y,
				b.freeze_orientation.z
			));

			L.append(TextFormat(" %f %f %f",
				b.freeze_linear_vel.x,
				b.freeze_linear_vel.y,
				b.freeze_linear_vel.z
			));

			A.append(TextFormat(" %f %f %f",
				b.freeze_angular_vel.x,
				b.freeze_angular_vel.y,
				b.freeze_angular_vel.z
			));
		}
		tempframe.append(TextFormat("%s\n", P.c_str()));
		tempframe.append(TextFormat("%s\n", Q.c_str()));
		tempframe.append(TextFormat("%s\n", L.c_str()));
		tempframe.append(TextFormat("%s\n", A.c_str()));
	}

	if (0 < game_frame) {
		std::ofstream tempreplayfile("tempreplayfile.txt", std::ios::app);
		tempreplayfile << tempframe;
		tempreplayfile.close();
	} else {
		std::ofstream tempreplayfile("tempreplayfile.txt");
		tempreplayfile << tempframe;
		tempreplayfile.close();
	}

	std::ofstream tempframefile("tempframefile.txt");
	tempframefile << tempframe << std::endl;
	tempframefile.close();
}

void Replay::RecordFrame()
{
	RecordFrame(0);
}

void Replay::Play(int game_frame)
{
	for (auto& p : frames[game_frame].players) {
		for (auto& j : p.joint) {
			switch(j.state) {
			case RELAX: {
				j.state = RELAX;
				j.TriggerPassiveState(0.00);
			} break;
			case HOLD: {
				j.state = HOLD;
				j.TriggerPassiveState(j.strength);
			} break;
			case FORWARD: {
				j.state = FORWARD;
				j.TriggerActiveState(1.00);
			} break; 
			case BACKWARD: {
				j.state = BACKWARD;
				j.TriggerActiveState(-1.00);
			} break;
			}
		
			switch(j.state_alt) {
			case RELAX: {
				j.state_alt = RELAX;
				j.TriggerPassiveStateAlt(0.00);
			} break;
			case HOLD: {
				j.state_alt = HOLD;
				j.TriggerPassiveStateAlt(j.strength_alt);
			} break;
			case FORWARD: {
				j.state_alt = FORWARD;
				j.TriggerActiveStateAlt(1.00);
			} break;
			case BACKWARD: {
				j.state_alt = BACKWARD;
				j.TriggerActiveStateAlt(-1.00);
			} break;
			}
		}
	}
}

void Replay::Save(std::string replay_name)
{
	std::ofstream savedreplayfile(replay_name.append(".txt"));
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
