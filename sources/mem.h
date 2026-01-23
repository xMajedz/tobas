#pragma once
#include "common.h"

class Arena
{
public:
	Arena() {}
	~Arena() {}

	Arena(size_t sz);

	uintptr_t allocate(size_t sz);

	void clear();

	uintptr_t buffer();
	size_t offset();
	size_t size();
private:
	uintptr_t m_ptr = 0;
	size_t m_offset = 0;
	size_t m_size = 0;
};
