#include "color.hpp"

/**
 * Copy constructor
 */
Color::Color(const Color& rhs)
{
	if (this == &rhs)	return;
	
	m_r = rhs.m_r;
	m_g = rhs.m_g;
	m_b = rhs.m_b;
}

uint8_t Color::clamp(float val) const
{
	int ival = static_cast<int>(val * UINT8_MAX);
	return val < 0 ? 0 : ival > UINT8_MAX ? UINT8_MAX : ival;
}

std::ostream &operator<<(std::ostream &os, Color const &c)
{ 
    return os << "[" << c.r() << ", " << c.g() << ", " << c.b() << "]";
}
