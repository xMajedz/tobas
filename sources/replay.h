#pragma once
#include "common.h"
#include "game.h"

struct FrameData
{
	std::vector<Player> players;
};

namespace Replay 
{
	static std::map<int, FrameData> frames;
	
	void RecordFrame(int game_frame);
	void RecordFrame();
	void Play(int game_frame);

	void Save(std::string replay_name);
	void Save();

	FrameData Get(int frame);
	std::map<int, FrameData> &Get();
}
