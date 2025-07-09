#pragma once
#include "netcode_main.h"

typedef float  f32_t;
typedef double f64_t;

namespace Client
{
	int Connect();
	void Disconnect();
	void Close();

	void Update(f64_t t, f32_t dt);

	void SkipLocalSim();

	void Ready();
}
