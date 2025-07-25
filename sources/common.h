#pragma once
#include "ode/ode.h"

#include "raylib/raylib.hpp"

#include <cstdint>

#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>

#include <iostream>
#define LOG_PREFIX "LOG: "
#define LOG_SUFFIX ""
#define LOG(X) std::cout << LOG_PREFIX << X << LOG_SUFFIX << std::endl;
#define LOG_A LOG("A")
#define LOG_B LOG("B")
#define LOG_C LOG("C")
#define LOG_D LOG("D")
