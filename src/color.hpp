#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include <iostream>
#include "defs.hpp"

/**
 * Class defining a color type.
 */
class Color
{
public:
	/// Constructors
	Color() : m_r(0), m_g(0), m_b(0) {}
	Color(int r, int g, int b) : m_r(r), m_g(g), m_b(b) {}
	Color(float r, float g, float b) : m_r(r * 255), m_g(g * 255), m_b(b * 255) {}

	/// Destructors
	~Color() {}

	/// Copy constructor
	Color(const Color& rhs);
	
	/// Overridden operators
	inline Color& operator=(const Color& rhs)
		{
			if (this == &rhs)	return *this;
	
			m_r = rhs.m_r;
			m_g = rhs.m_g;
			m_b = rhs.m_b;
	
			return *this;
		}

	Color operator*(const float scale) const;
	Color& operator*=(const float scale);

	/// Set and get operators.
	uint8_t r() const { return m_r; }
	uint8_t g() const { return m_g; }
	uint8_t b() const { return m_b; }
	void r(const uint8_t _r) { m_r = _r; }
	void g(const uint8_t _g) { m_g = _g; }
	void b(const uint8_t _b) { m_b = _b; }
	
private:
	uint8_t clamp(int val) const;

	/// Properties of the color
	uint8_t m_r;
	uint8_t m_g;
	uint8_t m_b;
};

std::ostream &operator<<(std::ostream &os, Color const &c);

#endif // __COLOR_HPP__

