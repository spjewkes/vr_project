#ifndef __VECTOR3D_HPP__
#define __VECTOR3D_HPP__

#include <cmath>

/**
 * Class defining a 3d vector type.
 */
class Vector3d
{
public:
	/// Constructors
	Vector3d() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
	Vector3d(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}
	
	/// Destructors
	~Vector3d() {}

	/// Copy constructor
	Vector3d(const Vector3d& rhs);
	
	/// Overridden operators
	inline Vector3d& operator=(const Vector3d& rhs)
		{
			if (this == &rhs)	return *this;
	
			m_x = rhs.m_x;
			m_y = rhs.m_y;
			m_z = rhs.m_z;
	
			return *this;
		}

	inline Vector3d operator+(const Vector3d& rhs) const
	{
		return Vector3d(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z);
	}

	inline Vector3d operator-(const Vector3d& rhs) const
	{
		return Vector3d(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z);
	}

	inline Vector3d operator*(const float scale) const
	{
		return Vector3d(m_x * scale, m_y * scale, m_z * scale);
	}

	inline Vector3d operator/(const float scale) const
	{
		if (scale == 0.0f)
		{
			return Vector3d(0.0f, 0.0f, 0.0f);
		}
		else
		{
			return Vector3d(m_x / scale, m_y / scale, m_z / scale);
		}
	}

	inline Vector3d& operator+=(const Vector3d& rhs)
	{
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		m_z += rhs.m_z;
		return *this;
	}

	inline Vector3d& operator-=(const Vector3d& rhs)
	{
		m_x -= rhs.m_x;
		m_y -= rhs.m_y;
		m_z -= rhs.m_z;
		return *this;
	}

	inline Vector3d& operator*=(const float scale)
	{
		m_x *= scale;
		m_y *= scale;
		m_z *= scale;
		return *this;
	}

	inline Vector3d& operator/=(const float scale)
	{
		if (scale == 0.0f)
		{
			m_x = 0.0f;
			m_y = 0.0f;
			m_z = 0.0f;
		}
		else
		{
			m_x /= scale;
			m_y /= scale;
			m_z /= scale;
		}
	
		return *this;
	}

	inline Vector3d operator-() const
	{
		return Vector3d(-m_x, -m_y, -m_z);
	}

	bool operator==(const Vector3d& rhs) const;
	bool operator!=(const Vector3d& rhs) const;
	bool operator<(const Vector3d& rhs) const;
	bool operator<=(const Vector3d& rhs) const;
	bool operator>(const Vector3d& rhs) const;
	bool operator>=(const Vector3d& rhs) const;
	
	/// Set and get operators.
	float x() const { return m_x; }
	float y() const { return m_y; }
	float z() const { return m_z; }
	void x(const float x) { m_x = x; }
	void y(const float y) { m_y = y; }
	void z(const float z) { m_z = z; }
	
	/// Get length of vector.
	float length() const;
	
	/// Get squared length of vector.
	float length2() const;
	
	/// Normalize the vector.
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

	/// Cross product of vectors.
	inline Vector3d cross(const Vector3d& rhs) const
	{
		return Vector3d(m_y * rhs.m_z - m_z * rhs.m_y,
						m_z * rhs.m_x - m_x * rhs.m_z,
						m_x * rhs.m_y - m_y * rhs.m_x);
	}

	/// Dot product of vectors.
	inline float dot(const Vector3d& rhs) const
	{
		return m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z;
	}

	/// Reflect vector to normal.
	inline Vector3d reflect(const Vector3d& normal) const
	{
		float k = dot( normal ) * 2.0f;
		return Vector3d( m_x - ( normal.m_x * k ),
						 m_y - ( normal.m_y * k ),
						 m_z - ( normal.m_z * k ) );
	}
	
private:
	/// Properties of the 3d vector
	float m_x;
	float m_y;
	float m_z;
};

#endif // __VECTOR3D_HPP__
