#pragma once
#include "ode/ode.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <map>
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

template <typename T> struct array {
private:
	T* m_data;
	size_t m_size;
public:
	array(){}
	array(int size)
	{
		m_data = new T[size];
		m_size = size;
	};

	array(int size, std::vector<T> v)
	{
		m_data = new T[size];
		m_size = size;
		for (int i = 0; i < m_size; i += 1) {
			*(m_data + i) = v[i];
		}
	};

	~array()
	{
//LOG("array_destruct")
	};

	T* data() {
		return m_data;
	};

	size_t size() {
		return m_size;
	};

	T operator[](int index)
	{
		return *(m_data + index);
	};
};
