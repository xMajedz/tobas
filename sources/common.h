#pragma once
#include "ode/ode.h"

#include "raylib/raylib.hpp"

#include <cstdint>

#include <map>
#include <unordered_map>
#include <array>
#include <vector>
#include <string>
#include <string_view>

struct vec3
{
	double x;
	double y;
	double z;
};

struct vec4
{
	double x;
	double y;
	double z;
	double w;
};

#include <iostream>
#define LOG_PREFIX "LOG: "
#define LOG_SUFFIX ""
#define LOG(X) std::cout << LOG_PREFIX << X << LOG_SUFFIX << std::endl;
#define LOG_A LOG("A")
#define LOG_B LOG("B")
#define LOG_C LOG("C")
#define LOG_D LOG("D")
