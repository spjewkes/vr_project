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

/**
 * Operators.
 */
Color Color::operator*(const float scale) const
{
	return Color(clamp(static_cast<int>(m_r) * scale),
				 clamp(static_cast<int>(m_g) * scale),
				 clamp(static_cast<int>(m_b) * scale));
}

Color& Color::operator*=(const float scale)
{
	m_r = clamp(static_cast<int>(m_r) * scale);
	m_g = clamp(static_cast<int>(m_g) * scale);
	m_b = clamp(static_cast<int>(m_b) * scale);

	return *this;
}

uint8_t Color::clamp(int val) const
{
	return val < 0 ? 0 : val > UINT8_MAX ? UINT8_MAX : val;
}

std::ostream &operator<<(std::ostream &os, Color const &c)
{ 
    return os << "[" << c.r() << ", " << c.g() << ", " << c.b() << "]";
}
