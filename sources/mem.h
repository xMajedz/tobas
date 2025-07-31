#pragma once
#include "common.h"

class Arena
{
public:
	Arena() {}
	~Arena() {}
	Arena(size_t sz);

	template <typename T> T* allocate(size_t sz)
	{
		size_t offset = m_offset;
		size_t new_offset = m_offset + sizeof(T) * sz;
		if (m_size < new_offset) {
			LOG(m_size)
			LOG(new_offset)
			LOG("we are full yo")
			m_offset -= m_offset;
			return (T*)m_ptr;
		} else if (0 < sz) {
			m_offset = new_offset;
			return new ((uint8_t*)m_ptr + offset) T;
		} else {
			return 0;
		}
	}

	void clear();
	size_t offset();
	size_t size();
private:
	size_t m_offset = 0;

	uintptr_t m_ptr = 0;
	size_t m_size = 0;
};
