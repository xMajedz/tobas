#include "mem.h"

Arena::Arena(size_t sz)
{
	m_size = sz;
	m_ptr = (uintptr_t)(new uint8_t[sz]);
}

uintptr_t Arena::allocate(size_t sz)
{
	uint8_t* offset_ptr = (uint8_t*)(m_ptr + m_offset);
	size_t new_offset = m_offset + sz;

	if (m_size < new_offset) {
		clear();
		return m_ptr;
	} else if (0 < sz) {
		m_offset = new_offset;
		return (uintptr_t)(new (offset_ptr) uint8_t[sz]);
	} else {
		return 0;
	}
}

void Arena::clear()
{
	m_offset = 0;
}

size_t Arena::offset()
{
	return m_offset;
}

size_t Arena::size()
{
	return m_size;
}

