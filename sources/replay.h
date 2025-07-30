#pragma once
#include "common.h"
#include "game.h"

struct FramePlayer
{
	size_t j_count = 0;
	size_t b_count = 0;

	uint8_t* J = nullptr;
	uint8_t* B = nullptr;

	float* Q = nullptr;
	float* P = nullptr;
	float* L = nullptr;
	float* A = nullptr;
};

struct FrameData
{
	size_t p_count = 0;
	FramePlayer* players = nullptr;
};

namespace Replay 
{
	static std::map<int, FrameData> frames;
	
	void WriteMetaData();
	void WriteFrameData(std::string data);
	void WriteReplayData(std::string data);
	
	void RecordFrame(int game_frame);
	void RecordFrame();
	void Play(int game_frame);

	void Save(std::string replay_name);
	void Save();

	void Destroy();

	FrameData Get(int frame);
	std::map<int, FrameData> &Get();
}
