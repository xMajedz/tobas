#pragma once
#include "ode/ode.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <string>
#include <vector>
#include <map>

#include <iostream>
#define LOG_PREFIX "LOG: "
#define LOG_SUFFIX ""
#define LOG(X) std::cout << LOG_PREFIX << X << LOG_SUFFIX << std::endl;
#define LOG_A LOG("A")
#define LOG_B LOG("B")
#define LOG_C LOG("C")

template <typename T> struct array {
	T* start;
	size_t length;

	array(){}
	array(int size)
	{
		start = new T[size];
		length = size;
	};

	array(int size, std::vector<T> v)
	{
		start = new T[size];
		length = size;
		for (int i = 0; i < length; i += 1) {
			*(start + i) = v[i];
		}
	}

	~array()
	{
//LOG("array_destruct")
	};

	T operator[](int index)
	{
		return *(start + index);
	};
};
