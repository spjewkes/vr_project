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
	Color() : m_r(0.0f), m_g(0.0f), m_b(0.0f) {}
	Color(float r, float g, float b) : m_r(r), m_g(g), m_b(b) {}

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

	inline Color operator+(const Color& rhs) const
	{
		return Color(m_r + rhs.m_r, m_g + rhs.m_g, m_b + rhs.m_b);
	}

	inline Color operator-(const Color& rhs) const
	{
		return Color(m_r - rhs.m_r, m_g - rhs.m_g, m_b - rhs.m_b);
	}

	inline Color operator*(const float scale) const
		{
			return Color(m_r * scale, m_g * scale, m_b * scale);
		}
	
	inline Color operator*(const Color& rhs) const
		{
			return Color(m_r * rhs.m_r, m_g * rhs.m_g, m_b * rhs.m_b);
		}
	
	Color& operator+=(const Color& rhs)
		{
			m_r += rhs.m_r;
			m_g += rhs.m_g;
			m_b += rhs.m_b;
			return *this;
		}

	Color& operator-=(const Color& rhs)
		{
			m_r -= rhs.m_r;
			m_g -= rhs.m_g;
			m_b -= rhs.m_b;
			return *this;
		}

	Color& operator*=(const float scale)
		{
			m_r *= scale;
			m_g *= scale;
			m_b *= scale;
			return *this;
		}

	Color& operator*=(const Color& rhs)
		{
			m_r *= rhs.m_r;
			m_g *= rhs.m_g;
			m_b *= rhs.m_b;
			return *this;
		}

	/// Set and get operators.
	uint8_t r() const { return clamp(m_r * UINT8_MAX); }
	uint8_t g() const { return clamp(m_g * UINT8_MAX); }
	uint8_t b() const { return clamp(m_b * UINT8_MAX); }
	void r(float v) { m_r = v; }
	void g(float v) { m_g = v; }
	void b(float v) { m_b = v; }

private:
	uint8_t clamp(float val) const;

	/// Properties of the color
	float m_r;
	float m_g;
	float m_b;
};

std::ostream &operator<<(std::ostream &os, Color const &c);

#endif // __COLOR_HPP__

