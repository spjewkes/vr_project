#ifndef __VECTOR3D_HPP__
#define __VECTOR3D_HPP__

#include <cassert>
#include <iostream>
#include <cmath>
#include "defs.hpp"

/**
 * @brief Mutable three-component vector used for positions, directions, and
 * transforms.
 *
 * Rotation angles are expressed in degrees. Arithmetic is component-wise except
 * for the explicit dot(), cross(), and reflect() operations.
 */
class Vector3d
{
  public:
	Vector3d() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
	Vector3d(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}

	~Vector3d() {}

	Vector3d(const Vector3d &rhs);

	inline Vector3d &operator=(const Vector3d &rhs)
	{
		if (this == &rhs) return *this;

		m_x = rhs.m_x;
		m_y = rhs.m_y;
		m_z = rhs.m_z;

		return *this;
	}

	inline Vector3d operator+(const Vector3d &rhs) const
	{
		return Vector3d(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z);
	}

	inline Vector3d operator-(const Vector3d &rhs) const
	{
		return Vector3d(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z);
	}

	inline Vector3d operator*(const float scale) const
	{
		return Vector3d(m_x * scale, m_y * scale, m_z * scale);
	}

	inline Vector3d operator*(const Vector3d &scale) const
	{
		return Vector3d(m_x * scale.x(), m_y * scale.y(), m_z * scale.z());
	}

	inline Vector3d &operator+=(const Vector3d &rhs)
	{
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		m_z += rhs.m_z;
		return *this;
	}

	inline Vector3d &operator-=(const Vector3d &rhs)
	{
		m_x -= rhs.m_x;
		m_y -= rhs.m_y;
		m_z -= rhs.m_z;
		return *this;
	}

	inline Vector3d &operator*=(const float scale)
	{
		m_x *= scale;
		m_y *= scale;
		m_z *= scale;
		return *this;
	}

	inline Vector3d &operator*=(const Vector3d &scale)
	{
		m_x *= scale.x();
		m_y *= scale.y();
		m_z *= scale.z();
		return *this;
	}

	inline Vector3d &operator/=(const float scale)
	{
		assert(scale != 0.0f);
		m_x /= scale;
		m_y /= scale;
		m_z /= scale;
		return *this;
	}

	inline Vector3d operator-() const
	{
		return Vector3d(-m_x, -m_y, -m_z);
	}

	/**
	 * @name Component-wise comparisons
	 *
	 * Ordering operators require the relationship to hold for every component;
	 * they do not define lexicographical or magnitude ordering.
	 * @{
	 */
	bool operator==(const Vector3d &rhs) const;
	bool operator!=(const Vector3d &rhs) const;
	bool operator<(const Vector3d &rhs) const;
	bool operator<=(const Vector3d &rhs) const;
	bool operator>(const Vector3d &rhs) const;
	bool operator>=(const Vector3d &rhs) const;
	/** @} */

	float x() const
	{
		return m_x;
	}
	float y() const
	{
		return m_y;
	}
	float z() const
	{
		return m_z;
	}
	void x(const float x)
	{
		m_x = x;
	}
	void y(const float y)
	{
		m_y = y;
	}
	void z(const float z)
	{
		m_z = z;
	}

	/**
	 * @brief Rotate this vector around the origin.
	 * @param angle Rotation around the X, Y, and Z axes in degrees.
	 */
	inline void rotate(const Vector3d &angle)
	{
		// Convert x, y and z coords from degrees to radians
		// and apply them to both the cosine and the sine functions
		float cax = cos(angle.x() / 57.295779513082321f);
		float sax = sin(angle.x() / 57.295779513082321f);
		float cay = cos(angle.y() / 57.295779513082321f);
		float say = sin(angle.y() / 57.295779513082321f);
		float caz = cos(angle.z() / 57.295779513082321f);
		float saz = sin(angle.z() / 57.295779513082321f);

		// perform the rotation
		float A = m_x;
		float B = (m_y * cax) + (m_z * sax);
		float C = (m_z * cax) - (m_y * sax);
		float D = (cay * A) - (say * C);
		m_z = (say * A) + (cay * C);
		m_y = (B * caz) - (D * saz);
		m_x = (D * caz) + (B * saz);
	}

	/** @brief Apply independent scale factors to the three components. */
	inline void scale(float x, float y, float z)
	{
		m_x *= x;
		m_y *= y;
		m_z *= z;
	}

	/** @return Euclidean length of the vector. */
	float length() const;

	/** @return Squared Euclidean length without a square-root operation. */
	float length2() const;

	/**
	 * @brief Normalize this vector in place.
	 * @return Length before normalization.
	 * @note A zero-length vector remains the zero vector.
	 */
	inline float normalize()
	{
		float len = length();

		if (len > 0.0f)
		{
			float invLength = 1.0f / len;
			m_x *= invLength;
			m_y *= invLength;
			m_z *= invLength;
		}
		else
		{
			m_x = 0.0f;
			m_y = 0.0f;
			m_z = 0.0f;
		}

		return len;
	}

	/** @return Cross product of this vector and rhs. */
	inline Vector3d cross(const Vector3d &rhs) const
	{
		return Vector3d((m_y * rhs.m_z) - (m_z * rhs.m_y), (m_z * rhs.m_x) - (m_x * rhs.m_z),
		                (m_x * rhs.m_y) - (m_y * rhs.m_x));
	}

	/** @return Dot product of this vector and rhs. */
	inline float dot(const Vector3d &rhs) const
	{
		return (m_x * rhs.m_x) + (m_y * rhs.m_y) + (m_z * rhs.m_z);
	}

	/**
	 * @brief Reflect this vector about a surface normal.
	 * @param normal Unit-length surface normal.
	 * @return Reflected vector.
	 */
	inline Vector3d reflect(const Vector3d &normal) const
	{
		float k = dot(normal) * 2.0f;
		return Vector3d(m_x - (normal.m_x * k), m_y - (normal.m_y * k), m_z - (normal.m_z * k));
	}

  private:
	float m_x;
	float m_y;
	float m_z;
};

std::ostream &operator<<(std::ostream &os, Vector3d const &v);

#endif // __VECTOR3D_HPP__
