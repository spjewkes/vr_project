#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include <cstdint>
#include <iostream>
#include "defs.hpp"

/**
 * @brief Floating-point RGB colour used by materials and lighting.
 *
 * Components conventionally use the range 0.0 to 1.0. Arithmetic and setters
 * may produce values outside that range; conversion to an 8-bit channel clamps
 * the result rather than modifying the stored value.
 */
class Color
{
  public:
	Color() : m_r(0.0f), m_g(0.0f), m_b(0.0f) {}
	Color(float r, float g, float b) : m_r(r), m_g(g), m_b(b) {}

	~Color() {}

	Color(const Color &rhs);

	inline Color &operator=(const Color &rhs)
	{
		if (this == &rhs) return *this;

		m_r = rhs.m_r;
		m_g = rhs.m_g;
		m_b = rhs.m_b;

		return *this;
	}

	inline Color operator+(const Color &rhs) const
	{
		return Color(m_r + rhs.m_r, m_g + rhs.m_g, m_b + rhs.m_b);
	}

	inline Color operator-(const Color &rhs) const
	{
		return Color(m_r - rhs.m_r, m_g - rhs.m_g, m_b - rhs.m_b);
	}

	inline Color operator*(const float scale) const
	{
		return Color(m_r * scale, m_g * scale, m_b * scale);
	}

	inline Color operator*(const Color &rhs) const
	{
		return Color(m_r * rhs.m_r, m_g * rhs.m_g, m_b * rhs.m_b);
	}

	Color &operator+=(const Color &rhs)
	{
		m_r += rhs.m_r;
		m_g += rhs.m_g;
		m_b += rhs.m_b;
		return *this;
	}

	Color &operator-=(const Color &rhs)
	{
		m_r -= rhs.m_r;
		m_g -= rhs.m_g;
		m_b -= rhs.m_b;
		return *this;
	}

	Color &operator*=(const float scale)
	{
		m_r *= scale;
		m_g *= scale;
		m_b *= scale;
		return *this;
	}

	Color &operator*=(const Color &rhs)
	{
		m_r *= rhs.m_r;
		m_g *= rhs.m_g;
		m_b *= rhs.m_b;
		return *this;
	}

	/** @return Red component converted and clamped to an 8-bit channel. */
	std::uint8_t r() const
	{
		return clamp(m_r * UINT8_MAX);
	}
	/** @return Green component converted and clamped to an 8-bit channel. */
	std::uint8_t g() const
	{
		return clamp(m_g * UINT8_MAX);
	}
	/** @return Blue component converted and clamped to an 8-bit channel. */
	std::uint8_t b() const
	{
		return clamp(m_b * UINT8_MAX);
	}
	/** @brief Set the un-clamped floating-point red component. */
	void r(float v)
	{
		m_r = v;
	}
	/** @brief Set the un-clamped floating-point green component. */
	void g(float v)
	{
		m_g = v;
	}
	/** @brief Set the un-clamped floating-point blue component. */
	void b(float v)
	{
		m_b = v;
	}

  private:
	std::uint8_t clamp(float val) const;

	float m_r;
	float m_g;
	float m_b;
};

std::ostream &operator<<(std::ostream &os, Color const &c);

#endif // __COLOR_HPP__
