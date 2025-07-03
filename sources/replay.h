#pragma once
#include "common.h"
#include "game.h"

struct FrameData : public FreezeData {
	std::map<std::string, Player> player;
};

namespace Replay 
{
	static std::map<int, FrameData> s_frames;
	
	void Record(int game_frame);
	void Record();
	void Play(int game_frame);

	void Save(std::string replay_name);
	void Save();

	FrameData Get(int game_frame);
	std::map<int, FrameData> Get();
}
