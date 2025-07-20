#pragma once

typedef float  f32_t;
typedef double f64_t;

namespace Client
{
	int Connect();
	int Connect(const char* host, int port);
	void Disconnect();
	void Close();

	void Update(f64_t t, f32_t dt);

	void SkipLocalSim();

	void Ready();
}
