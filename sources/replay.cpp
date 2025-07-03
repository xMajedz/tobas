#include "replay.h"

#include <fstream>

void Replay::Record(int game_frame)
{
	/*std::string tempframe = "F";
	tempframe.append(TextFormat(" %d\n",game_frame));
	for (auto const& [player_name, p] : players) {
		std::string J = "J";
		std::string P = "P";
		std::string Q = "Q";
		std::string L = "L";
		std::string A = "A";

		s_frames[game_frame].player[player_name] = p;

		for (auto const& [joint_name, j] : p.joint) {
			J.append(TextFormat(" %d %d", j.state, j.state_alt));
		}
		tempframe.append(TextFormat("%s\n", J.c_str()));

		for (auto const& [body_name, b] : p.body) {
			P.append(TextFormat(" %f %f %f",
				b.freeze.position.x,
				b.freeze.position.y,
				b.freeze.position.z
			));

			Q.append(TextFormat(" %f %f %f %f",
				b.freeze.orientation.w,
				b.freeze.orientation.x,
				b.freeze.orientation.y,
				b.freeze.orientation.z
			));

			L.append(TextFormat(" %f %f %f",
				b.freeze.linear_vel.x,
				b.freeze.linear_vel.y,
				b.freeze.linear_vel.z
			));

			A.append(TextFormat(" %f %f %f",
				b.freeze.angular_vel.x,
				b.freeze.angular_vel.y,
				b.freeze.angular_vel.z
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
	tempframefile.close();*/
}

void Replay::Record()
{
	Record(0);
}

void Replay::Play(int game_frame)
{
	const auto& frame = Get(game_frame);
	/*for (auto& [player_name, p] : frame.player) {
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
	}*/
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

FrameData Replay::Get(int game_frame)
{
	return s_frames[game_frame];
}

std::map<int, FrameData> Replay::Get()
{
	return s_frames;
}
