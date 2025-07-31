#include "mem.h"

Arena::Arena(size_t sz)
{
	m_size = sz;
	m_ptr = (uintptr_t)(new uint8_t[sz]);
}

void Arena::clear()
{
	m_offset -= m_offset;
}

size_t Arena::offset()
{
	return m_offset;
}

size_t Arena::size()
{
	return m_size;
}

