#pragma once
#include "common.h"
#include "game.h"
#include "mem.h"

struct FramePlayer
{
	size_t j_count = 0;
	size_t b_count = 0;

	uint8_t* J = nullptr;
	uint8_t* B = nullptr;

	double* Q = nullptr;
	double* P = nullptr;
	double* L = nullptr;
	double* A = nullptr;
};

struct FrameData
{
	size_t p_count = 0;
	FramePlayer* players = nullptr;
	//FrameObjet* objects = nullptr;
};

namespace Replay 
{
	static Arena* storage = nullptr;
	/*
	 * map because frame gapes
	 */
	static std::map<int, FrameData> frames;

	void Init();
	void Close();
	
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
